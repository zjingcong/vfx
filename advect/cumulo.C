
# include "DoAdvection.h"

using namespace std;


void printHelp()
{
    cout << "[Usage] ./cumulo frame_id output_path cfg_path" << endl;
}


int main(int argc, char* argv[])
{
    // cmd line parser
    int frameid = 0;
    string output_path;
    string cfg_path;
    // string output_path = "../output";
    if (argc >= 4)
    {
        frameid = atoi(argv[1]);
        if (frameid >= 120 || frameid < 0)  {cout << "frame_id: 0~119" << endl; exit(0);}
        output_path = argv[2];
        cfg_path = argv[3];
    }
    else {printHelp();  exit(0);}

    setCfgPath(cfg_path);
    createBunnyCumulo(frameid, output_path);

    return 0;
}
