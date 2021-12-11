#include <SDL.h>
#include <glew.h>
#include <iostream>
#include <fstream>
#include <string>


using namespace std;

string LoadShader(string fileName);

int main(int argc = 0, char** argv = nullptr) {

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        cout << "SDL initialized successfully \n";
    }

    int flags = SDL_WINDOW_OPENGL;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "SDL failed to initialize" << endl;
        return 1;
    }
    ///////////SETTING UP SDL/////////////
    //Create a simple window
    int width = 640;
    int height = 480;
    unsigned int center = SDL_WINDOWPOS_CENTERED;
    SDL_Window* Window = SDL_CreateWindow("The IM PIC ABLE crown!", center, center, width, height, SDL_WINDOW_OPENGL);
    //SDL_WINDOW_OPENGL is a u32 flag !


    ////Create an OpenGL compatible context to let glew draw on it
    SDL_GLContext Context = SDL_GL_CreateContext(Window);


    /////////SETTING UP OPENGL WITH GLEW///
    //Initialize glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "Glew failed to initialize\n";
    }


    // Get info
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version supported: " << version << endl;

    // Tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

    // Set viewport and clear color
    glViewport(0, 0, width, height);
    glClearColor(1.0f-0.173f, 1.0f-0.173f, 1.0f-0.173f, 1.0f);

    // Load

    //Describe the shape by its vertices

    float vertices[] = {

        -0.5f, -0.5f, 0.0f,         1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,         0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,         0.0f, 0.0f, 1.0f,

         0.75f, 0.66f, 0.0f,        1.0f, 0.0f, 0.0f,
        -0.75f,-0.66f, 0.0f,        0.0f, 1.0f, 0.0f,
         0.75f,-0.66f, 0.0f,        0.0f, 0.0f, 1.0f,

        -0.4f, 0.4f, 0.0f,          1.0f, 0.0f, 1.0f,
         0.4f, 0.4f, 0.0f,           1.0f, 1.0f, 1.0f,
         0.0f,-0.4f, 0.0f,          0.0f, 1.0f, 1.0f
 
    };

    //Create an ID to be given at object generation
    unsigned int vbo = 0;


    //Pass how many buffers should be created and the reference of the ID to get the value set
    glGenBuffers(1, &vbo);


    //Binds the buffer linked to this ID to the vertex array buffer to be rendered. Put 0 instead of vbo to reset the value.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //Finally send the vertices array in the array buffer (linked to vbo)
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    string vs = LoadShader("VertexShader.txt");
    const char* vertexShaderSource = vs.c_str();
    string fs = LoadShader("FragmentShader.txt");
    const char* fragmentShaderSource = fs.c_str();


    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);


    //now that we have a vertex shader, let’s put the code text inside
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);


    //aaaaand… Compile !
    glCompileShader(vertexShader);


    //Do the same with the fragment shader !
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();



    //now attach shaders to use to the program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    //and link it 
    glLinkProgram(shaderProgram);

    //now that the program is complete, we can use it 
    glUseProgram(shaderProgram);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // ..:: Initialization code (done once (unless your object frequently changes)) :: ..
    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);

    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. then set our vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Game loop
    bool isRunning = true;
    while (isRunning) {
        // Inputs
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;

            default:
                break;
            }
        }
        // Update


        // Draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

        // Draw here
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        float timeValue =(float)SDL_GetTicks() / 1000;
        float redColor = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUseProgram(shaderProgram);
        glUniform4f(vertexColorLocation, redColor, 1.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 4, 3);

        glUseProgram(shaderProgram);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram);
        glDrawArrays(GL_TRIANGLES, 3, 3);
        SDL_GL_SwapWindow(Window); // Swapbuffer
    }


    // Quit
    SDL_DestroyWindow(Window);
    SDL_GL_DeleteContext(Context);

    return 0;
}


string LoadShader(string fileName) {
    ifstream myFile;
    myFile.open(fileName);
    if (myFile.fail()) {
        cerr << "Error - failed to open " << fileName << endl;
    }
    string fileText = "";
    string line = "";
    while (getline(myFile, line)) {
        fileText += line + '\n';
    }
    myFile.close();
    return fileText;


}



