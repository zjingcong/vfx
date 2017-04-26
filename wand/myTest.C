
# include <iostream>
# include "DoWandTest.h"
# include "CmdLineFind.h"
# include "Particles.h"

using namespace std;
using namespace lux;


int main(int argc, char* argv[])
{
//    CmdLineFind clf( argc, argv );
//    string output_path = clf.find("-output", "../output", "output image path");
//    int frame_id = clf.find("-id", 0, "frame id");
//    testCreateWispLines(frame_id, output_path);

    WispCloud wispCloud(5);
    // test particles
    for (int i = 0; i < 20; ++i)
    {
        wispCloud.spendTime();
        cout << "time_step: " << i << endl;
        wispCloud.testSys();
        cout << "---------------------" << endl;
    }

    return 0;
}
