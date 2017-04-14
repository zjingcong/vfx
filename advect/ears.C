
# include "DoAdvection.h"

using namespace std;


void printHelp()
{
    cout << "[Usage] ./ears cfg_path grid_path output_path start_id end_id" << endl;
    cout << "cfg_path: config file path" << endl;
    cout << "grid_path: .vdb grids file path" << endl;
    cout << "output_path: images output path" << endl;
}


int main(int argc, char* argv[])
{
    // cmdline parser
    int start_id, end_id;
    string cfg_path;
    string grid_path;
    string output_path;
    if (argc >= 6)
    {
        cfg_path = argv[1];
        grid_path = argv[2];
        output_path = argv[3];
        start_id = atoi(argv[4]);
        end_id = atoi(argv[5]);
        
        if (start_id >= 120 || start_id < 0 || end_id >= 120 || end_id < 0)
            {cout << "frame_id: 0~199" << endl; exit(0);}
    }
    else {printHelp();  exit(0);}

    setCfgPath(cfg_path);
    setGridsOutPath(grid_path);
    createEar(start_id, end_id, output_path);

    return 0;
}
