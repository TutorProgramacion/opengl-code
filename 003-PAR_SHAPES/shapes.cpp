#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include <glad\glad.h>
#include <glfw\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#define PAR_SHAPES_T GLuint
#define PAR_SHAPES_IMPLEMENTATION
#include "par_shapes.h"

GLuint init_shader();

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

GLuint loadParShapes(GLuint &count)
{  
    GLuint buffer[3], vao;
    
    par_shapes_mesh* shape = par_shapes_create_trefoil_knot(30, 100, 0.8);
    par_shapes_scale(shape, 2.0f, 2.0f, 2.0f);

    count = shape->ntriangles * 3;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(3, buffer);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, shape->npoints * 3 * sizeof(float), shape->points, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, shape->npoints * 3 * sizeof(float), shape->normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape->ntriangles * 3 * sizeof(PAR_SHAPES_T), shape->triangles, GL_STATIC_DRAW);

    glBindVertexArray(0);

    par_shapes_free_mesh(shape);

    return vao;
}

GLuint loadParShapesAndNormal(GLuint &count)
{
    GLuint buffer[3], vao;

    par_shapes_mesh* shape = par_shapes_create_dodecahedron();
    par_shapes_translate(shape, -2.0f, 0, 0);

    std::vector<glm::vec3> position, normal;

    PAR_SHAPES_T const* triangle = shape->triangles;

    for (int f = 0; f < shape->ntriangles; f++, triangle += 3) {
        float const* pa = shape->points + 3 * triangle[0];
        float const* pb = shape->points + 3 * triangle[1];
        float const* pc = shape->points + 3 * triangle[2];

        glm::vec3 p0(pa[0], pa[1], pa[2]);
        glm::vec3 p1(pb[0], pb[1], pb[2]);
        glm::vec3 p2(pc[0], pc[1], pc[2]);

        position.push_back(p0);
        position.push_back(p1);
        position.push_back(p2);

        glm::vec3 n = glm::normalize(glm::cross(p1 - p0, p2 - p0));

        normal.push_back(n);
        normal.push_back(n);
        normal.push_back(n);
    }

    count = position.size();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(3, buffer);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(glm::vec3), position.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(glm::vec3), normal.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    par_shapes_free_mesh(shape);

    return vao;
}

void drawShape_0() {
    static GLuint count_0 = 0;
    static GLuint vao_0 = 0;

    if (vao_0 == 0) {
        vao_0 = loadParShapesAndNormal(count_0);
    }

    glBindVertexArray(vao_0);
    glDrawArrays(GL_TRIANGLES, 0, count_0);
}

void drawShape_1() {
    static GLuint count_1 = 0;
    static GLuint vao_1 = 0;

    if (vao_1 == 0) {
        vao_1 = loadParShapes(count_1);
    }

    glBindVertexArray(vao_1);
    glDrawElements(GL_TRIANGLES, count_1, GL_UNSIGNED_INT, NULL);
}

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(1280, 768, "Par Shapes :: OpenGL Moderno", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    GLuint shader = init_shader();
    GLuint u_proj = glGetUniformLocation(shader, "proj");
    GLuint u_view = glGetUniformLocation(shader, "view");
    GLuint u_modl = glGetUniformLocation(shader, "model");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.45f, 0.75f, 0.90f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 proj = glm::perspective(45.0f, 1280.0f / 768.0f, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(4, 5, 3), glm::vec3(0), glm::vec3(0, 1, 0));
        glm::mat4 modl;

        glUseProgram(shader);

        glUniformMatrix4fv(u_proj, 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(u_modl, 1, GL_FALSE, glm::value_ptr(modl));

        drawShape_0();
        drawShape_1();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


GLuint init_shader() {

    const GLchar* vertex_shader_source = R"(
        #version 330 core                                
                                                 
        in vec3 position;            
        in vec3 normal;            
    
        uniform mat4 proj;                                             
        uniform mat4 view;
        uniform mat4 model; 

        uniform vec3 lightPosition = vec3(0.0, 3.0, 3.0);
               
        out VS_OUT {
	        vec3 N;
	        vec3 L;
	        vec3 V;
        } vs_out;              
									         
        void main(void) {  
	        gl_Position = proj * view * model * vec4(position, 1.0);

        	mat4 mv_matrix = view * model;

        	vec3 light_pos = vec3(view * vec4(lightPosition, 1.0));
	        vec4 view_pos = mv_matrix * vec4(position, 1.0);

        	vs_out.N = mat3(transpose(inverse(mv_matrix))) * normal;
	        vs_out.L = light_pos - view_pos.xyz;
	        vs_out.V = -view_pos.xyz;
        }              
    )";

    const GLchar* fragment_shader_source = R"( 
        #version 330 core                        
           
        out vec4 final_color; 

        uniform vec3 lightColor  = vec3(0.75, 0.75, 0.75);
        uniform vec3 objectColor = vec3(0.75, 0.75, 0.75);

        in VS_OUT {
	        vec3 N;
	        vec3 L;
	        vec3 V;
        } fs_in;

        const float k0 = 1.000;
        const float k1 = 0.090;
        const float k2 = 0.032;
		                            
        void main(void) 
        {  
	        float d = length(fs_in.L);

            vec3 N = normalize(fs_in.N);
	        vec3 L = normalize(fs_in.L);
	        vec3 V = normalize(fs_in.V);
	        vec3 H = normalize(L + V);
	
            float intensity = 0.80;
	        float diffuse = clamp(dot(N, L), 0, 1);
	        float specular = pow(clamp(dot(N, H), 0, 1), 32);
	        float attenuation = 1.0 / (k0 + (k1 * d) + (k2 * d * d));

            diffuse  *= attenuation * intensity;
		    specular *= attenuation * intensity;

            vec3 ambientColor  = vec3(0.1);
		    vec3 diffuseColor  = objectColor * lightColor * diffuse;
		    vec3 specularColor = objectColor * lightColor * specular;
	
		    final_color = vec4(ambientColor + diffuseColor + specularColor, 1.0);
        }                                        
    )";

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    glLinkProgram(shader_program);

    GLint link_status = 0;
    GLint log_length = 0;

    glGetProgramiv(shader_program, GL_LINK_STATUS, &link_status);
    glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &log_length);

    if (link_status == GL_FALSE) {
        auto logger = std::make_unique<GLchar[]>(log_length);
        glGetProgramInfoLog(shader_program, log_length, NULL, logger.get());

        std::cerr << logger.get() << std::endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}