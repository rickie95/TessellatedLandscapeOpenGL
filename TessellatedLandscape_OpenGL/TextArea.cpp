#include "TextArea.h"

void TextArea::setScreen(screen* ss) {
	SCREEN = ss;
}

screen* TextArea::SCREEN = nullptr;

float TextArea::vertices[] = {
	-1.0f, -1.0f,
	1.0f, -1.0f, 
	1.0f,  1.0f, 

	1.0f, 1.0f,
	-1.0f, 1.0f,
	-1.0f, -1.0f
};

glm::vec4 TextArea::bgColor = { 0.1, 0.1, 0.1, 0.9 };

TextArea::TextArea(int x, int y, int w, int h=0)
{
	if (SCREEN == nullptr)
		std::cout << "TEXTAREA :: ERROR :: SCREEN NOT SET " << std::endl;

	rows = new std::vector<guiElement>();

	this->addElement("// TESSELLATED LANDSCAPE");

	textColor = glm::vec3(1.0f, 1.0f, 1.0f);

	visible = true;
	float aspect = SCREEN->WIDTH / SCREEN->HEIGHT;
	this->width = w;
	this->height = h;
	
	shader_text = new Shader("TextShader.vert", "TextShader.frag");
	shader = new Shader("TextArea.vert", "TextArea.frag");
	points = new Object(vertices, 6*2, 2);
	points->setShader(shader);
	initFreeType();
}

void TextArea::calculateMatrix() {

	int numOfElem = rows->size();

	float width = (float)this->width / SCREEN->WIDTH;
	float height = (float)(numOfElem*20+5)/ SCREEN->HEIGHT;

	float offset_x = 5.0;
	float offset_y = 5.0;
	float x_pos = (float)(-SCREEN->WIDTH + offset_x) / SCREEN->WIDTH + width;
	float y_pos = (float)(SCREEN->HEIGHT - offset_y) / SCREEN->HEIGHT - height;

	transformation = glm::mat4(1);
	scaleMat = glm::scale(glm::mat4(1), glm::vec3(width, height, 0.0));

	transformation = glm::translate(transformation, glm::vec3((float)(x_pos), (float)(y_pos), 0.0));
}

TextArea::~TextArea()
{
	rows->clear();
}

void TextArea::setVisible(bool set)
{
	visible = set;
}

void TextArea::addElement(std::string name, float * value)
{
	guiElement el = guiElement( name, value );
	rows->push_back(el);
}

void TextArea::addElement(std::string name, double * value)
{
	guiElement el = guiElement(name, value);
	rows->push_back(el);
}

void TextArea::addElement(std::string name, int * value)
{
	guiElement el = guiElement(name, value);
	rows->push_back(el);
}

void TextArea::addElement(std::string name, std::string * value)
{
	guiElement el = guiElement(name, value);
	rows->push_back(el);
}


void TextArea::draw() {
	if (visible) {
		// Disegno lo sfondo
		shader->use();
		shader->setData("transformation", transformation);
		shader->setData("scale", scaleMat);
		shader->setData("bgColor", bgColor);
		points->drawObject();
		calculateMatrix();
		// Adesso le righe
		for (int i = 0; i < rows->size(); i++) {
			RenderText((*rows)[i].getString(), 10.0f, SCREEN->HEIGHT - 20 - (float)(i * 20), 0.35f, textColor);
		}
	}
}

void TextArea::initFreeType() {

	this->projection = glm::ortho(0.0f, static_cast<GLfloat>(SCREEN->WIDTH), 0.0f, static_cast<GLfloat>(SCREEN->HEIGHT));
	shader_text->use();
	glUniformMatrix4fv(glGetUniformLocation(shader_text->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	FT_Face face;
	if (FT_New_Face(ft, "Fonts/calibri.ttf", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
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
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void TextArea::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	// Activate corresponding render state	
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shader_text->use();
	glUniform3f(glGetUniformLocation(shader_text->ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos,     ypos,       0.0, 1.0 },
		{ xpos + w, ypos,       1.0, 1.0 },

		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos + w, ypos,       1.0, 1.0 },
		{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

guiElement::guiElement(std::string name, float * value){
	this->name = name;
	this->value_f = value;

}

guiElement::guiElement(std::string name, double * value) {
	this->name = name;
	this->value_d = value;
}

guiElement::guiElement(std::string name, int * value)
{
	this->name = name;
	value_i = value;
}

guiElement::guiElement(std::string name, std::string * value)
{
	this->name = name;
	value_s = value;
}

guiElement::guiElement(std::string name)
{
	this->name = name;
}

std::string guiElement::getString()
{
	std::string ss = this->name;
	std::ostringstream streamObj;
	streamObj << std::fixed;
	streamObj << std::setprecision(2);

	if(value_d != NULL)
		streamObj << (*value_d);
	if(value_f != NULL)
		streamObj << (*value_f);
	if(value_i != NULL)
		streamObj << (*value_i);
	if (value_s != NULL)
		streamObj << (*value_s);

	ss += " " + streamObj.str();
	
	return ss;
}
