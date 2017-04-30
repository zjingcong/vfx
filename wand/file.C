
# include <iostream>
# include "DoWand.h"
# include "CmdLineFind.h"

using namespace std;
using namespace lux;


int main(int argc, char* argv[])
{
    CmdLineFind clf( argc, argv );
    string output_path = clf.find("-output", "../output", "output image path");
    int frame_id = clf.find("-id", 0, "frame id");
    string grid_path = clf.find("-grid", "/DPA/jedi/zjingcong/grids/wand_grids", "openvdb grid files path");

    createWispLines(frame_id, output_path, grid_path);

    return 0;
}