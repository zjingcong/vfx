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


typedef  struct
{
    float octaves;
    float freq;
    float fjump;
    float clump;
} wispWedgeParms;


std::vector<noiseWedgeParms> noiseParmsList;
std::vector<pyroWedgeParms> pyroParmsList;
std::vector<wispWedgeParms> wispParmsList;


// noise wedge parms
float noise_octaves_list[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
float noise_freq_list[5] = {0.7293, 1.0343, 1.87823, 2.143023, 2.846};
float noise_f_jump_list[5] = {0.65, 1.7, 1.9, 2.1, 2.4};
float noise_fade_list[4] = {0.5, 1.0, 1.8, 2.5};


// pyroclastic wedge parms
float pyro_octaves_list[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
float pyro_freq_list[5] = {0.57434, 1.2343, 2.32432, 3.434345, 5.546};
float pyro_f_jump_list[4] = {0.7, 1.5, 2.0, 2.6};
float pyro_gamma_list[5] = {0.33333, 0.7, 1.0, 1.5, 2.0};


// wisp wedge parms
float wisp_octaves_list[5] = {0.4, 0.9, 1.4, 1.9, 2.4};
float wisp_freq_list[5] = {0.57434, 1.5343, 3.02432, 4.334345, 5.546};
float wisp_f_jump_list[4] = {1.0, 1.5, 2.0, 2.5};
float wisp_clump_list[5] = {1.0, 0.556, 0.23333, 0.123, 0.0};


void getNoiseParms(string output_path)
{
    char file_name[1024];
    sprintf(file_name, "%s/noiseParms.json", output_path.c_str());
    ofstream noiseParmsFile(file_name);
    noiseParmsFile << "{" << endl;
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
                    char frameidinfo[1024];
                    sprintf(frameidinfo, "\"frame_id\": \"%04d\"", id);
                    char octavesinfo[1024];
                    sprintf(octavesinfo, "\"octaves\": \"%f\"", octaves);
                    char freqinfo[1024];
                    sprintf(freqinfo, "\"freq\": \"%f\"", freq);
                    char fjumpinfo[1024];
                    sprintf(fjumpinfo, "\"fjump\": \"%f\"", fjump);
                    char fadeinfo[1024];
                    sprintf(fadeinfo, "\"fade\": \"%f\"", fade);
                    char frameinfo[1024];
                    sprintf(frameinfo, "\"%04d\": {%s, %s, %s, %s, %s}", id, frameidinfo, octavesinfo, freqinfo, fjumpinfo, fadeinfo);
                    noiseParmsFile << frameinfo;
                    if (id < 499)   {noiseParmsFile << ", " << endl;}
                    id++;
                }
            }
        }
    }
    noiseParmsFile << "}" << endl;
    noiseParmsFile.close();
    std::cout << "Output parms: " << file_name << std::endl;
}


void getPyroParms(string output_path)
{
    char file_name[1024];
    sprintf(file_name, "%s/pyroParms.json", output_path.c_str());
    ofstream pyroParmsFile(file_name);
    pyroParmsFile << "{" << endl;
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
                    char frameidinfo[1024];
                    sprintf(frameidinfo, "\"frame_id\": \"%04d\"", id);
                    char octavesinfo[1024];
                    sprintf(octavesinfo, "\"octaves\": \"%f\"", octaves);
                    char freqinfo[1024];
                    sprintf(freqinfo, "\"freq\": \"%f\"", freq);
                    char fjumpinfo[1024];
                    sprintf(fjumpinfo, "\"fjump\": \"%f\"", fjump);
                    char gammainfo[1024];
                    sprintf(gammainfo, "\"gamma\": \"%f\"", gamma);
                    char frameinfo[1024];
                    sprintf(frameinfo, "\"%04d\": {%s, %s, %s, %s, %s}", id, frameidinfo, octavesinfo, freqinfo, fjumpinfo, gammainfo);
                    pyroParmsFile << frameinfo;
                    if (id < 499)   {pyroParmsFile << ", " << endl;}
                    id++;
                }
            }
        }
    }
    pyroParmsFile << "}" << endl;
    pyroParmsFile.close();
    std::cout << "Output parms: " << file_name << std::endl;
}


void getWispParms(string output_path)
{
    char file_name[1024];
    sprintf(file_name, "%s/wispParms.json", output_path.c_str());
    ofstream wispParmsFile(file_name);
    wispParmsFile << "{" << endl;
    int id = 0;
    for (int i = 0; i < 4; i++)
    {
        float fjump = wisp_f_jump_list[i];
        for (int j = 0; j < 5; j++)
        {
            float clump = wisp_clump_list[j];
            for (int k = 0; k < 5; k++)
            {
                float octaves = wisp_octaves_list[k];
                for (int m = 0 ; m < 5; m++)
                {
                    float freq = wisp_freq_list[m];
                    wispWedgeParms para;
                    para.clump = clump;
                    para.freq = freq;
                    para.fjump = fjump;
                    para.octaves = octaves;
                    wispParmsList.push_back(para);
                    char frameidinfo[1024];
                    sprintf(frameidinfo, "\"frame_id\": \"%04d\"", id);
                    char octavesinfo[1024];
                    sprintf(octavesinfo, "\"octaves\": \"%f\"", octaves);
                    char freqinfo[1024];
                    sprintf(freqinfo, "\"freq\": \"%f\"", freq);
                    char fjumpinfo[1024];
                    sprintf(fjumpinfo, "\"fjump\": \"%f\"", fjump);
                    char clumpinfo[1024];
                    sprintf(clumpinfo, "\"clump\": \"%f\"", clump);
                    char frameinfo[1024];
                    sprintf(frameinfo, "\"%04d\": {%s, %s, %s, %s, %s}", id, frameidinfo, octavesinfo, freqinfo, fjumpinfo, clumpinfo);
                    wispParmsFile << frameinfo;
                    if (id < 499)   {wispParmsFile << ", " << endl;}
                    id++;
                }
            }
        }
    }
    wispParmsFile << "}" << endl;
    wispParmsFile.close();
    std::cout << "Output parms: " << file_name << std::endl;
}

# endif
