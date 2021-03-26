// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow *window;

// Include GLM
#include <glm/glm.hpp>
#include <shader.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

int main(void) {
  // Initialise GLFW
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    getchar();
    return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(1024, 768, "Tutorial 01", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr,
            "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
    getchar();
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    return -1;
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Dark blue background
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

////  	 Enable depth test
//  glEnable(GL_DEPTH_TEST);
////   Accept fragment if it closer to the camera than the former one
//  glDepthFunc(GL_LESS);

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  GLuint T1pid = LoadShaders("fstT.vertexshader", "default.fragmentshader");
  GLuint T2pid = LoadShaders("sndT.vertexshader", "default.fragmentshader");

  GLuint MatrixID = glGetUniformLocation(T1pid, "MVP");

  // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
  // Camera matrix
  vec4  eye = vec4(4, 3, -3, 1);

  glm::mat4 rot = glm::rotate(mat4(), glm::radians(0.4f), vec3(0, 1, 0));

  glm::mat4 View = glm::lookAt(
      vec3(eye), // Camera is at (4,3,-3), in World Space
      glm::vec3(0, 0, 0), // and looks at the origin
      glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
  );

  // Model matrix : an identity matrix (model will be at the origin)
  glm::mat4 Model = glm::mat4(1.0f);
  // Our ModelViewProjection : multiplication of our 3 matrices
  glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around


  static const GLfloat f_vertex_buffer_data[] = {
      0.5f, -0.5f, 1.0f,
      0.0f, 0.0f, 0.0f,
      0.5f, 0.5f, 1.0f,
  };

  static const GLfloat s_vertex_buffer_data[] = {
      0.3f, -0.4f, 0.0f,
      0.1f, 0.1f, 1.0f,
      0.3f, 0.4f, 0.0f,
  };

  GLuint f_vertexbuffer;
  glGenBuffers(1, &f_vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, f_vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(f_vertex_buffer_data), f_vertex_buffer_data, GL_STATIC_DRAW);

  GLuint s_vertexbuffer;
  glGenBuffers(1, &s_vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, s_vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertex_buffer_data), s_vertex_buffer_data, GL_STATIC_DRAW);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  do {
    eye = eye * rot;

    View = glm::lookAt(
        vec3(eye), // Camera is at (4,3,-3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    MVP = Projection * View * Model;

    glClear(GL_COLOR_BUFFER_BIT);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glUseProgram(T1pid);

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, f_vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void *) 0            // array buffer offset
    );

    glDrawArrays(GL_TRIANGLES, 0, 1 * 3);

    glDisableVertexAttribArray(0);

    glUseProgram(T2pid);

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, s_vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void *) 0            // array buffer offset
    );

    glDrawArrays(GL_TRIANGLES, 0, 1 * 3);

    glDisableVertexAttribArray(0);


    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

  } // Check if the ESC key was pressed or the window was closed
  while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
      glfwWindowShouldClose(window) == 0);


  glDeleteBuffers(1, &f_vertexbuffer);
  glDeleteBuffers(1, &s_vertexbuffer);
  glDeleteProgram(T1pid);
  glDeleteProgram(T2pid);
  glDeleteVertexArrays(1, &VertexArrayID);

  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  return 0;
}

