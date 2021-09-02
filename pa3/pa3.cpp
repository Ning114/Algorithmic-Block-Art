// File:        pa3.cpp
// Author:      cs221 instructors
// Date:        2021-03-03
// Description: Partial test of PA3 functionality
//              Reads Point data from external files
//              Produces PNG images of the point sets

#include "sqtree.h"

int main() {

  // read in image
  PNG orig;
  // orig.readFromFile("images/orig/4by4test.png");
  orig.readFromFile("images/orig/stanley-totem-poles.png");
  /*
  orig.readFromFile("images/orig/small2.png");
  orig.readFromFile("images/orig/pacman.png");
  orig.readFromFile("images/orig/huckleberry_hound.png");
  orig.readFromFile("images/orig/rocky.png");
  orig.readFromFile("images/orig/rocky_bullwinkle.png");
  orig.readFromFile("images/orig/colour.png");
  orig.readFromFile("images/orig/klee-flora-on-sand.png");
  orig.readFromFile("images/orig/stanley-totem-poles.png");
  */
  
  // use it to build a sqtree (try other tolerances)
  SQtree tree = SQtree(orig, 20000);

  // copy the tree (to show you can)
  // SQtree tree2 = SQtree(tree);

  cout << "tree size: ";
  cout << tree.size() << endl;

  // render the sqtree
  PNG pic = tree.render();
  

  // write it out
  pic.writeToFile("images/out/small1.png");




  return 0;
}
