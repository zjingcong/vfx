
# include "DoAdvection.h"

using namespace std;


void printHelp()
{
    cout << "[Usage] ./ears frame_id grid_path output_path" << endl;
    cout << "grid_path: .vdb grids file path" << endl;
    cout << "output_path: images output path" << endl;
}


int main(int argc, char* argv[])
{
    // cmdline parser
    int frameid = 0;
    string grid_path;
    string output_path;
    if (argc >= 4)
    {
        frameid = atoi(argv[1]);
        if (frameid >= 120 || frameid < 0)  {cout << "frame_id: 0~199" << endl; exit(0);}
        grid_path = argv[2];
        output_path = argv[3];
    }
    else {printHelp();  exit(0);}

    setGridsOutPath(grid_path);
    createEar(frameid, output_path);

    return 0;
}
