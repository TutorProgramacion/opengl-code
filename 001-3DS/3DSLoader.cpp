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

GLuint init_shader();

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void readChunck(std::ifstream &file, int CHUNCK_ID)
{
    uint16_t chunck = 0;
    uint32_t lenght = 0;
    
    do {
        file.read(reinterpret_cast<char*>(&chunck), 2);
        file.read(reinterpret_cast<char*>(&lenght), 4);

        if (chunck != CHUNCK_ID) file.seekg(lenght - 6, std::ios_base::cur);

    } while (chunck != CHUNCK_ID);
}

void readChunckChar(std::ifstream& file) 
{
    char c = ' ';
    while (c != '\0') {
        file.get(c);
        std::cout << c;
    } 
}

void readChunckVertices(std::ifstream& file, std::vector<glm::vec3>& vertices) 
{
    uint16_t num_vertices = 0;
    file.read(reinterpret_cast<char*>(&num_vertices), 2);

    vertices.resize(num_vertices);
    file.read(reinterpret_cast<char*>(&vertices[0].x), num_vertices * sizeof(glm::vec3));
}

void readChunckFaces(std::ifstream& file, std::vector<GLuint>& faces) 
{
    uint16_t num_faces = 0;
    file.read(reinterpret_cast<char*>(&num_faces), 2);

    faces.reserve(num_faces * 3);

    for (uint16_t i = 0; i < num_faces; i++)
    {
        uint16_t temp_faces[4];
        file.read(reinterpret_cast<char*>(&temp_faces), sizeof(short) * 4);

        faces.push_back(temp_faces[0]);
        faces.push_back(temp_faces[1]);
        faces.push_back(temp_faces[2]);
    }
}

GLuint load3DS(const std::string& filename, GLuint& count)
{
    std::ifstream file{ filename, std::ios_base::binary };

    if (!file) {
        std::cerr << "No se puede abrir el archivo: " << filename << std::endl;
        return 0;
    }

    std::vector<glm::vec3> vertices, normals;
    std::vector<GLuint> faces;

    readChunck(file, 0x4D4D);
        readChunck(file, 0x3D3D);
            readChunck(file, 0x4000); readChunckChar(file);
                readChunck(file, 0x4100);
                    readChunck(file, 0x4110); readChunckVertices(file, vertices);
                    readChunck(file, 0x4120); readChunckFaces(file, faces);

    file.close();

    normals.resize(vertices.size());
    count = faces.size();

    for (size_t f = 0; f < faces.size(); f += 3) {

        GLuint f0 = faces[f + 0];
        GLuint f1 = faces[f + 1];
        GLuint f2 = faces[f + 2];

        glm::vec3 v0 = vertices[f0];
        glm::vec3 v1 = vertices[f1];
        glm::vec3 v2 = vertices[f2];
        
        glm::vec3 e1 = v1 - v0;
        glm::vec3 e2 = v2 - v0;
        
        glm::vec3 N = glm::cross(e1, e2);

        normals[f0] += N;
        normals[f1] += N;
        normals[f2] += N;
    }

    for (size_t i = 0; i < normals.size(); i++) {
        normals[i] = glm::normalize(normals[i]);
    }

    GLuint buffer[3], vao;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(3, buffer);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(GLuint), faces.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    return vao;
}

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(1280, 768, "3DS :: OpenGL Moderno", NULL, NULL);

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

    double start = glfwGetTime();

    GLuint count = 0;
    GLuint vao = load3DS(PROJECT_SOURCE_DIR "../resources/model/duck.3ds", count);

    std::cout << "loaded in " << glfwGetTime() - start << " segundos" << std::endl;

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

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL);

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