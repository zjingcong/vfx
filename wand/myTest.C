
# include <iostream>
# include "DoWand.h"
# include "CmdLineFind.h"
# include "Particles.h"

using namespace std;
using namespace lux;


int main(int argc, char* argv[])
{
//    CmdLineFind clf( argc, argv );
//    string output_path = clf.find("-output", "../output", "output image path");
//    int frame_id = clf.find("-id", 0, "frame id");
//    // testCreateWispLines(frame_id, output_path);
//
//    WispCloud wispCloud(20, 0.8, Vector(0.0, 1.0, 0.0));
//    for (int i = 0; i < 40; ++i)
//    {
//        wispCloud.spendTime();
//        cout << "time_step: " << i << endl;
//        wispCloud.testSys();
//        cout << "---------------------" << endl;
//    }

    CmdLineFind clf( argc, argv );
    string output_path = clf.find("-output", "../output", "output image path");
    int frame_id = clf.find("-id", 0, "frame id");

    createWispLines(frame_id, output_path);

    return 0;
}
