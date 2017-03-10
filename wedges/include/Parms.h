# ifndef __PARMS_H__
# define  __PARMS_H__

# include <vector>
# include <fstream>

typedef struct
{
    float octaves;
    float freq;
    float fjump;
    float fade;
} noiseWedgeParms;

std::vector<noiseWedgeParms> noiseParmsList;

// noise wedge parms
float noise_octaves_list[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
float noise_freq_list[5] = {1.0, 1.2343, 2.32432, 3.434345, 5.546};
float noise_f_jump_list[5] = {1.5, 1.7, 1.9, 2.1, 2.4};
float noise_fade_list[4] = {0.5, 0.7, 1.0, 1.5};


void getNoiseParms()
{
    ofstream noiseParmsFile("../output/noiseParms.log");
    int id = 0;
    for (int i = 0; i < 4; i++)
    {
        float fade = noise_fade_list[i];
        for (int j = 0; j < 5; j++)
        {
            float freq = noise_freq_list[j];
            for (int k = 0; k < 5; k++)
            {
                float fjump = noise_f_jump_list[k];
                for (int m = 0 ; m < 5; m++)
                {
                    float octaves = noise_octaves_list[m];
                    noiseWedgeParms para;
                    para.fade = fade;
                    para.freq = freq;
                    para.fjump = fjump;
                    para.octaves = octaves;
                    noiseParmsList.push_back(para);
                    noiseParmsFile << "frame_id " << id << " | fade: " << fade
                                   << ", freq: " << freq << ", fjump: " << fjump
                                   << ", octaves: " << octaves << endl;
                    id++;
                }
            }
        }
    }
    noiseParmsFile.close();
}

# endif
