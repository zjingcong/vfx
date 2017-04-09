
# include "DoAdvection.h"

using namespace std;


void printHelp()
{
    cout << "[Usage] ./cumulo frame_id [output_path]" << endl;
}


int main(int argc, char* argv[])
{
    // cmd line parser
    int frameid = 0;
    string output_path = "../output";
    if (argc >= 2)
    {
        frameid = atoi(argv[1]);
        if (frameid >= 120 || frameid < 0)  {cout << "frame_id: 0~119" << endl; exit(0);}
        if (argc == 3)  {output_path = argv[2];}
    }
    else {printHelp();  exit(0);}

//    createCMGrid();
    createBunnyCumulo(frameid, output_path);

    return 0;
}
