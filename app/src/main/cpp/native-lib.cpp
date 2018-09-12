#include "com_example_testgles3_GLUtils.h"
#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

static GLfloat vertices[] = {
        1.f,-.5f,1.f,
        1.f,-.5f,-1.f,
        -1.f,-.5f,-1.f,
        -1.f,-.5f,1.f,
        0.f,.5f,0.f
};

static GLfloat colors[] = {
        1.f,0.f,0.f,1.f,
        0.f,1.f,0.f,1.f,
        0.f,0.f,1.f,1.f,
        1.f,1.f,0.f,1.f,
        1.f,1.f,1.f,1.f
};

static GLubyte indices[] = {
        4,0,1,
        4,1,2,
        4,2,3,
        4,3,0,
        0,3,2,
        0,2,1
};

static int w, h;

static GLint angleX = 0, angleY = 0;

static GLuint VAO_ID, VBO_ID, EBO_ID;

static const char * const vertexShaderSource =
"attribute vec3 position;"
"attribute vec4 color;"
"varying vec4 vertexColor;"
"uniform mat4 Tcw;"
"void main() {"
"gl_Position=Tcw*vec4(position,1.0);"
"vertexColor=color;"
"}";

static GLuint vertexShaderId;
static GLuint positionLocation, colorLocation;

static const char * const fragmentShaderSource =
"precision mediump float;"
"varying vec4 vertexColor;"
"void main() {"
"gl_FragColor = vertexColor;"
"}";
static GLuint fragmentShaderId;

static GLuint programId;

static std::string errorInfo;

bool check(GLuint id, GLenum id_type, GLenum target_type) {
    int success;
    char infoLog[512];
    glGetShaderiv(id, target_type, &success);
    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        errorInfo = "Fail to ";
        if (target_type == GL_LINK_STATUS)
            errorInfo += "link program :";
        else if(target_type == GL_COMPILE_STATUS){
            errorInfo += "compile ";
            if (id_type == GL_VERTEX_SHADER)
                errorInfo += "vertex_shader :";
            else if (id == GL_FRAGMENT_SHADER)
                errorInfo += "fragment_shader :";
        }
        errorInfo += std::string(infoLog);
        return false;
    }
    return true;
}

JNIEXPORT jboolean JNICALL Java_com_example_testgles3_GLUtils_initialize
        (JNIEnv *, jclass) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    /* create and compile vertex shader */
    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    if(!vertexShaderId) {
        errorInfo = "Fail to create vertex_shader .";
        return (jboolean)false;
    }
    glShaderSource(vertexShaderId, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShaderId);
    if(!check(vertexShaderId, GL_VERTEX_SHADER, GL_COMPILE_STATUS))
        return (jboolean)false;

    /* create and compile fragment shader */
    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    if(!fragmentShaderId) {
        errorInfo = "Fail to create fragment_shader .";
        return (jboolean)false;
    }
    glShaderSource(fragmentShaderId, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShaderId);
    if(!check(fragmentShaderId, GL_FRAGMENT_SHADER, GL_COMPILE_STATUS))
        return (jboolean)false;

    /* create program and link shader */
    programId = glCreateProgram();
    if(!programId) {
        errorInfo = "Fail to create program .";
        return (jboolean)false;
    }
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);
    if(!check(programId, 0, GL_LINK_STATUS))
        return (jboolean)false;

    /* shader objects can be deleted after linking */
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    glUseProgram(programId);
    positionLocation = (GLuint)glGetAttribLocation(programId, "position");
    colorLocation = (GLuint)glGetAttribLocation(programId, "color");

    glGenVertexArrays(1, &VAO_ID);
    glGenBuffers(1, &VBO_ID);
    glGenBuffers(1, &EBO_ID);

    glBindVertexArray(VAO_ID);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
    glBufferData(GL_ARRAY_BUFFER, 35 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 15 * sizeof(GLfloat), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), 20 * sizeof(GLfloat), colors);

    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)(15 * sizeof(GLfloat)));
    glEnableVertexAttribArray(colorLocation);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 18 * sizeof(GLubyte), indices, GL_STATIC_DRAW);
    return (jboolean)true;
}

JNIEXPORT jstring JNICALL Java_com_example_testgles3_GLUtils_getErrors
        (JNIEnv * env, jclass) {
    return env->NewStringUTF(errorInfo.c_str());
}

JNIEXPORT void JNICALL Java_com_example_testgles3_GLUtils_resize
        (JNIEnv *, jclass, jint width, jint height) {
    glViewport(0, 0, w = width, h = height);
}

JNIEXPORT void JNICALL Java_com_example_testgles3_GLUtils_display
        (JNIEnv *, jclass) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(45.f), 1.f*w/h, 0.1f, 100.f);

    glm::mat4 view = glm::mat4(1.f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 model = glm::mat4(1.f);
    model = glm::rotate(model, glm::radians(GLfloat(angleX)), glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, glm::radians(GLfloat(angleY)), glm::vec3(1.f, 0.f, 0.f));

    glm::mat4 Tcw = glm::mat4(1.f);
    Tcw *= projection;
    Tcw *= view;
    Tcw *= model;
    GLint TcwLocation = glGetUniformLocation(programId, "Tcw");
    glUniformMatrix4fv(TcwLocation, 1, GL_FALSE, glm::value_ptr(Tcw));

    glBindVertexArray(VAO_ID);
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_BYTE, NULL);
}

JNIEXPORT void JNICALL Java_com_example_testgles3_GLUtils_update
        (JNIEnv *, jclass, jfloat angle, jint rotationAxis) {
    if(rotationAxis == 0)
        angleY += (int)angle;
    else
        angleX += (int)angle;
}

JNIEXPORT void JNICALL Java_com_example_testgles3_GLUtils_reset
        (JNIEnv *, jclass) {
    angleX = angleY = 0;
}