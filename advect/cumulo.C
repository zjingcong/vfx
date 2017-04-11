
# include "DoAdvection.h"

using namespace std;


void printHelp()
{
    cout << "[Usage] ./cumulo frame_id cfg_path grid_path output_path" << endl;
    cout << "cfg_path: config file path" << endl;
    cout << "grid_path: .vdb grids file path" << endl;
    cout << "output_path: images output path" << endl;
}


int main(int argc, char* argv[])
{
    // cmd line parser
    int frameid = 0;
    string cfg_path;
    string grid_path;
    string output_path;
    if (argc >= 5)
    {
        frameid = atoi(argv[1]);
        if (frameid >= 120 || frameid < 0)  {cout << "frame_id: 0~119" << endl; exit(0);}
        cfg_path = argv[2];
        grid_path = argv[3];
        output_path = argv[4];
    }
    else {printHelp();  exit(0);}

    setCfgPath(cfg_path);
    setGridsOutPath(grid_path);
    createBunnyCumulo(frameid, output_path);

    return 0;
}
