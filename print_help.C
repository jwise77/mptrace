#include <stdio.h>

void print_help(void)
{

  printf("\n"
"			   KEYSTROKE USAGE\n"
"========================================================================\n"
"Movement :: Arrow keys + PgUp/Down\n"
"\n"
"A = Move toward focus point\n"
"Z = Move away from focus point\n"
"\n"
",/. = Decrease / increase value of lowest isosurface\n"
"k/l = Decrease / increase value of highest isosurface\n"
"-/= = Decrease / increase separation of isosurfaces\n"
"9/0 = Decrease / increase thickness of isosurfaces\n"
"\n"
"C = Change color table\n"
"R = Change render mode\n"
"\n"
"H = Print this help :)\n"
	 );

  return;

}
