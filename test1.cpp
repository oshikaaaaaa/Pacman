#include <SFML/Graphics.hpp>

const std::string vertexShader = R"(
    void main()
    {
        gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
        gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    }
)";

const std::string fragmentShader = R"(
    uniform sampler2D texture;
    uniform float alpha;

    void main()
    {
        vec4 color = texture2D(texture, gl_TexCoord[0].xy);
        gl_FragColor = vec4(color.rgb, color.a * alpha);
    }
)";

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Fog Animation");
    window.setFramerateLimit(60);

    // Load the fog texture
    sf::Texture fogTexture;
    if (!fogTexture.loadFromFile("Resources/Images/fog_texture.png"))
    {
        return 1; // Return an error code if the texture fails to load
    }

    // Create the fog sprite
    sf::Sprite fogSprite;
    fogSprite.setTexture(fogTexture);

    // Load the shader
    sf::Shader fogShader;
    if (!fogShader.loadFromMemory(vertexShader, fragmentShader))
    {
        return 2; // Return an error code if the shader fails to load
    }

    float alpha = 0.0f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Update the animation
        alpha += 0.005f;
        if (alpha > 1.0f)
        {
            alpha = 0.0f;
        }

        fogShader.setUniform("texture", fogTexture);
        fogShader.setUniform("alpha", alpha);

        window.clear();
        window.draw(fogSprite, &fogShader);

        window.display();
    }

    return 0;
}
