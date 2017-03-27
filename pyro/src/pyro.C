
# include "DoPyros.h"

using namespace std;


void printHelp()
{
    cout << "[Usage] ./pyro frame_id [output_path]" << endl;
}


int main(int argc, char* argv[])
{
    // cmd line parser
    int frameid = 0;
    string output_path = "../output";
    if (argc >= 2)
    {
        frameid = atoi(argv[1]);
        if (frameid >= 300 || frameid < 0)  {cout << "frame_id: 0~299" << endl; exit(0);}
        if (argc == 3)  {output_path = argv[2];}
    }
    else {printHelp();  exit(0);}

    for (int i = 0; i < 4; ++i)
    {
        createPyroWedges(frameid + i, output_path);
    }

    return 0;
}
