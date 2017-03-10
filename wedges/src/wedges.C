
# include "DoWedges.h"

using namespace std;


void printHelp()
{
    cout << "[Usage] ./wedges wedge_type frame_id" << endl;
    cout << "====================================================" << endl;
    cout << "[wedge_type]: " << endl;
    cout << "\t -n: noise\n\t -w: wisp\n\t -p: pyroclastic" << endl;
}


int main(int argc, char* argv[])
{
    // cmd line parser
    // usage: ./wedges wedge_type frame_id
    int frameid = 0;
    string wedge_type;
    if (argc == 3)
    {
        wedge_type = argv[1];
        frameid = atoi(argv[2]);
        if (frameid >= 500 || frameid < 0)  {cout << "frame_id: 0~499" << endl; exit(0);}
        if (wedge_type != "-n" && wedge_type != "-w" && wedge_type != "-p")
            {printHelp();   exit(0);}
    }
    else {printHelp();  exit(0);}

    if (wedge_type == "-n")
    {
        getNoiseParms();
        createNoiseWedges(frameid);
    }
    else if (wedge_type == "-p")
    {
        createPyroWedges();
    }

    return 0;
}
