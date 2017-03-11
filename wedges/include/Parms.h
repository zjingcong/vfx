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


typedef struct
{
    float octaves;
    float freq;
    float fjump;
    float gamma;
} pyroWedgeParms;


std::vector<noiseWedgeParms> noiseParmsList;
std::vector<pyroWedgeParms> pyroParmsList;

// noise wedge parms
float noise_octaves_list[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
float noise_freq_list[5] = {0.82434, 1.2343, 2.32432, 3.434345, 5.546};
float noise_f_jump_list[5] = {1.5, 1.7, 1.9, 2.1, 2.4};
float noise_fade_list[4] = {0.5, 0.7, 1.0, 1.5};


// pyroclastic wedge parms
float pyro_octaves_list[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
float pyro_freq_list[5] = {0.82434, 1.2343, 2.32432, 3.434345, 5.546};
float pyro_f_jump_list[4] = {1.5, 1.9, 2.1, 2.4};
float pyro_gamma_list[5] = {0.2, 0.7, 1.0, 1.5, 2.0};


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


void getPyroParms()
{
    ofstream pyroParmsFile("../output/pyroParms.log");
    int id = 0;
    for (int i = 0; i < 5; i++)
    {
        float octaves = pyro_octaves_list[i];
        for (int j = 0; j < 5; j++)
        {
            float freq = pyro_freq_list[j];
            for (int k = 0; k < 4; k++)
            {
                float fjump = pyro_f_jump_list[k];
                for (int m = 0 ; m < 5; m++)
                {
                    float gamma = pyro_gamma_list[m];
                    pyroWedgeParms para;
                    para.gamma = gamma;
                    para.freq = freq;
                    para.fjump = fjump;
                    para.octaves = octaves;
                    pyroParmsList.push_back(para);
                    pyroParmsFile << "frame_id " << id << " | octaves: " << octaves
                                  << ", freq: " << freq << ", fjump: " << fjump
                                  << ", gamma: " << gamma << endl;
                    id++;
                }
            }
        }
    }
    pyroParmsFile.close();
}

# endif
