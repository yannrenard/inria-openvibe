// author: Guillaume Gibert
// affiliation: INSERM U821
// date: 10/12/2008




#ifndef __FreeType_H__
#define __FreeType_H__

//FreeType Headers
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

//OpenGL Headers 
#include <GL/gl.h>
#include <GL/glu.h>

//Some STL headers
#include <vector>
#include <string>
#include <iostream>
//Using the STL exception library increases the
//chances that someone else using our code will corretly
//catch any exceptions that we throw.
#include <stdexcept>


///Wrap everything in a namespace, that we can use common
///function names like "print" without worrying about
///overlapping with anyone else's code.
//~ namespace freetype {

//Inside of this namespace, give ourselves the ability
//to write just "vector" instead of "std::vector"
using std::vector;

//Ditto for string.
using std::string;

class FreeType
{
	public:
		FreeType();
		~FreeType();
	
	public:
		float h;			///< Holds the height of the font.
		GLuint * textures;	///< Holds the texture id's 
		GLuint list_base;	///< Holds the first display list id
	
		//~ //This holds all of the information related to any
		//~ //freetype font that we want to create.  
	//~ struct font_data {
		
	public:
		//The init function will create a font of
		//of the height h from the file fname.
		void init(const char * fname, unsigned int h);

		//Free all the resources assosiated with the font.
		void clean();
	
	//~ };

		//The flagship function of the library - this thing will print
		//out text at window coordinates x,y, using the font ft_font.
		//The current modelview matrix will also be applied to the text. 
		//~ void print(const font_data &ft_font, float x, float y, const char *fmt, ...) ;
		void print( float x, float y, const char *fmt, ...) ;
		int getNextPower2(int a);
		void makeDisplayList ( FT_Face face, char ch, GLuint list_base, GLuint * tex_base );
		void pushScreenCoordinateMatrix() ;
		void popProjectionMatrix() ;
};
	//~ }


#endif //__FreeType_H__
