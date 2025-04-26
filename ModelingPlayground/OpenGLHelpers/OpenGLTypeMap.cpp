#include "OpenGLTypeMap.h"

std::unordered_map<std::type_index, GLenum> OpenGLTypeMap::m_openGLTypeToEnumMap = {
    {std::type_index(typeid(GLbyte)), GL_BYTE},
    {std::type_index(typeid(GLubyte)), GL_UNSIGNED_BYTE},
    {std::type_index(typeid(GLshort)), GL_SHORT},
    {std::type_index(typeid(GLushort)), GL_UNSIGNED_SHORT},
    {std::type_index(typeid(GLint)), GL_INT},
    {std::type_index(typeid(GLuint)), GL_UNSIGNED_INT},
    {std::type_index(typeid(GLhalf)), GL_HALF_FLOAT},
    {std::type_index(typeid(GLfloat)), GL_FLOAT},
    {std::type_index(typeid(GLdouble)), GL_DOUBLE}
};
