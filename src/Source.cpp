#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <iostream>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "../header/stb_image.h"
#endif
#include "../header/tree/tree.h"
#include "../header/camera.h"
#include "../header/shader.h"
#include "../header/object/limb.h"
#include <map>
#include "../header/util.h"
#include <time.h>

#include <ft2build.h>
#include <freetype/freetype.h>
using namespace std;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void renderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);
void genPos1(vector <float>& allPos);
void genPos2(vector <float>& allPos);
using namespace std;
// settings
const unsigned int SCR_WIDTH = 450;
const unsigned int SCR_HEIGHT = 800;
const float zNear = 0.1f, zFar = 100.0f;
const bool drawLine = false;
float exposure = 1.0f;
struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size; // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    unsigned int Advance; // Offset to advance to next glyph
};
// camera
Camera camera(glm::vec3(10.0f, .0f, 0.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;
Model* model;
// timing
float deltaTime = 0.0f,lastFrame = 0.0f;
std::map<GLchar, Character> Characters;
float step = 0;
BaseObject* parent;
vector<float> allPos;
float scale = 1.0f;
vector<Limb*> allObj;
unsigned int VBO, VAO, maxStep = 7;
Tree tree("B", maxStep,15.0f,1.0f);
Shader* instanceShader;
Shader* modelShader;
Shader* lineShader;
unsigned int tVAO, tVBO;
int main() {

    camera.ProcessMouseMovement(-900, 0);
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    lineShader = new Shader("./src/shader/line.vs", "./src/shader/line.fs", "./src/shader/line.gs");
    instanceShader = new Shader("./src/shader/model_instance.vs", "./src/shader/model_instance.fs");
    modelShader = new Shader("./src/shader/model.vs", "./src/shader/model.fs");
    model = new Model("./res/cylinder_8v.obj");
	
	//tree.runAll();
	//cout << tree.lSystem.state << endl;
	//allObj = tree.draw();
    
    

    instanceShader->use();
    instanceShader->setVec3("lightPos",glm::vec3(0.0f) );
    instanceShader->setVec3("lightColor",glm::vec3(4,2,0.5));
   
    // compile and setup the shader
// ----------------------------
    Shader textShader("./src/shader/text.vs", "./src/shader/text.fs");
    glm::mat4 orthoProjection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    textShader.use();
    textShader.setMat4("projection", orthoProjection);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);


    // FreeType
    // --------
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    // find path to font
    std::string font_name = "./res/arial.ttf";
    if (font_name.empty())
    {
        std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
        return -1;
    }

    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }
    else {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    // configure VAO/VBO for texture quads
    // -----------------------------------
   
    glGenVertexArrays(1, &tVAO);
    glGenBuffers(1, &tVBO);
    glBindVertexArray(tVAO);
    glBindBuffer(GL_ARRAY_BUFFER, tVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //tree.draw();
    //parent = tree.drawNew();
   
    while (!glfwWindowShouldClose(window))
    {
        srand(time(NULL));
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
        glm::mat4 view = camera.GetViewMatrix();
        instanceShader->use();
        instanceShader->setMat4("projection", projection);
        instanceShader->setMat4("view", view);
        instanceShader->setVec3("viewPos", camera.Position );

        lineShader->use();
        lineShader->setMat4("view", view);
        lineShader->setMat4("projection", projection);

        step += deltaTime;
        allPos.clear();
        //genPos2(allPos);
        genPos1(allPos);
        processInput(window); 
        textShader.use();
        renderText(textShader, "#line " + std::to_string(allPos.size()/9), 15.0f, 15.0f, 0.4f, glm::vec3(1, 1, 1));

      
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glfwTerminate();
    return 0;
  
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}


void genPos1(vector <float> &allPos ) {
    if (step > maxStep)
        return;

    tree.runOneStep();
    allObj = tree.draw();
    for (unsigned int i = 0; i < allObj.size(); i++) {

        Limb* obj = allObj[i];
        //cout << obj->level << endl;
        /*if (obj->level > step)
            continue;*/
        allPos.push_back(obj->startPos.x * scale);
        allPos.push_back(obj->startPos.y * scale);
        allPos.push_back(obj->startPos.z * scale);

        allPos.push_back(obj->endPos.x * scale);
        allPos.push_back(obj->endPos.y * scale);
        allPos.push_back(obj->endPos.z * scale);
        glm::vec3 dir =  (obj->endPos - obj->startPos)* scale;
      
            
        if (obj->type == "bark") {
            allPos.push_back(1.0f);
            allPos.push_back(0.0f);
            allPos.push_back(0.0f);
        }
        else if (obj->type == "leaf") {
            allPos.push_back(0.0f);
            allPos.push_back(1.0f);
            allPos.push_back(0.0f);
        }
        else if (obj->type == "topLeaf") {
            allPos.push_back(0.0f);
            allPos.push_back(0.5f);
            allPos.push_back(0.0f);
        }
        else if (obj->type == "topBark") {
            allPos.push_back(0.0f);
            allPos.push_back(1.0f);
            allPos.push_back(0.0f);
        }
        //cout << obj->type << " " << Util::printVec3(obj->startPos * scale) << " -> " << Util::printVec3(obj->endPos * scale) << endl;
    }
  
    lineShader->use();
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * allPos.size(), allPos.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, allPos.size() / 9);
}

void genPos2(vector<float>& allPos) {
    //cout << "step" << step << endl;
    vector<glm::mat4> allMatrix;
    vector<glm::vec3> colorInstances;

    parent->parse(allPos,step,allMatrix);
    //cout << allPos.size()/9 << endl;
   /* for (int i = 0; i < allPos.size() ; i+=9) {
        cout << allPos[i] << " " << allPos[i + 1] << " " << allPos[i + 2] << " , "
            << allPos[i+3] << " " << allPos[i + 4] << " " << allPos[i + 5] << endl;
    }*/
    if (!drawLine) {
        int amount = allPos.size() / 9;
        vector<glm::mat4> modelMatrices;
        for (unsigned int pos = 0; pos < allPos.size(); pos += 9) {
            modelMatrices.push_back(allMatrix[pos / 9]);
            glm::mat4 m;
            m[0][0] = allPos[pos + 6];
            m[0][1] = allPos[pos + 7];
            m[0][2] = allPos[pos + 8];
            modelMatrices.push_back(m);
            //colorInstances.push_back(glm::vec3(, allPos[pos + 7], allPos[pos + 8]));
        }
        //return;
        // configure instanced array
        // -------------------------
        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, amount * (sizeof(glm::mat4)+sizeof(glm::mat4)), &(modelMatrices.data()[0]), GL_STATIC_DRAW);
        
        // set transformation matrices as an instance vertex attribute (with divisor 1)
        // note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
        // normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
        // -----------------------------------------------------------------------------------------------------------------------------------
       
        for (unsigned int i = 0; i < model->meshes.size(); i++)
        {
           
            unsigned int VAO = model->meshes[i].VAO;
            glBindVertexArray(VAO);
            // set attribute pointers for matrix (4 times vec4)
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::mat4), (void*)0);
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

            glEnableVertexAttribArray(7);
            glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 2*sizeof(glm::mat4), (void*)(4 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(8);
            glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::mat4), (void*)(5*sizeof(glm::vec4)));
            glEnableVertexAttribArray(9);
            glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::mat4), (void*)(6 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(10);
            glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::mat4), (void*)(7 * sizeof(glm::vec4)));

            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);

            glVertexAttribDivisor(7, 1);
            glVertexAttribDivisor(8, 1);
            glVertexAttribDivisor(9, 1);
            glVertexAttribDivisor(10, 1);

            glBindVertexArray(0);
        }

       
        instanceShader->use();
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, rock.textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.
        for (unsigned int i = 0; i < model->meshes.size(); i++)
        {
            glBindVertexArray(model->meshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(model->meshes[i].indices.size()), GL_UNSIGNED_INT, 0, amount);
            glBindVertexArray(0);
        }


    }
    else {


        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * allPos.size(), allPos.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindVertexArray(0);

        lineShader->use();
     
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, allPos.size() / 9);

    }

 }
void renderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
    // activate corresponding render state	
    shader.use();
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(tVAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, tVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
