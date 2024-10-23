#include <cmath>
#include <glad/glad.h>
#include "shader.h"
#include <glm/ext/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Circle 
{
	public:	
		unsigned int VBO, VAO;
		float radius;
		Shader& shader;
		glm::vec4 color;
		glm::vec3 pos;
		glm::vec3 pos_old;
		glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);

		Circle(float radius_, Shader& shaderD, glm::vec4 color_, glm::vec3 pos_) : shader(shaderD)
		{	


			this->color = color_;
			this->radius = radius_;
			this->shader = shader;
			this->pos = pos_;
			pos_old = pos_;
			const int divisions = 64;
			float vertices[divisions * 3];
			for (int i = 0; i < divisions; i++) 
			{
				vertices[i * 3] = radius * cos((float)i / divisions * 6.28f);
				vertices[i * 3 + 1] = radius * sin((float)i / divisions * 6.28f);
				vertices[i * 3 + 2] = 0.0f;
			}

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //STATIC_DRAW - data defined once, used many times

			// position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glBindVertexArray(0);
		}
		void draw(glm::mat4 &view, glm::mat4 &projection) {
			glBindVertexArray(VAO);
			this->shader.use();
			this->shader.setMat4("view", view);
			this->shader.setMat4("projection", projection);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, this->pos);
			this->shader.setMat4("model", model);
			this->shader.setVec4("ourColor", this->color);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 64);
		}


		void update(float dt) {
			const glm::vec3 velocity = this->pos - this->pos_old;
			this->pos_old = this->pos;
			this->pos = this->pos + velocity + this->acceleration * dt * dt;
			//acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		void applyConstraint() {
			const glm::vec2 constraintPos = glm::vec2(0.0f, 0.0f);
			const float constraintRadius = 0.95f;
			const glm::vec2 to_obj = glm::vec2(pos.x,pos.y) - constraintPos;
			const float dist = pow(to_obj.x*to_obj.x+to_obj.y*to_obj.y, 0.5);
			if (dist > constraintRadius - radius) {
				glm::normalize(to_obj);
				//const float velocity = pow((pos.x - pos_old.x)* (pos.x - pos_old.x)+ (pos.y - pos_old.y)* (pos.y - pos_old.y),0.5f);
				const float velocity = (pos.x-pos_old.x) * to_obj.x + (pos.y-pos_old.y) * to_obj.y;
				const glm::vec2 n = to_obj / dist;
				pos = glm::vec3(constraintPos + to_obj*velocity + n * (constraintRadius - radius),0.002f);
				//pos = glm::vec3(constraintPos - to_obj + n * (constraintRadius - radius), 0.002f);
			}
		}

		void setAcceleration(glm::vec3 acceleration_) {
			this->acceleration = acceleration_;
		}
		
};