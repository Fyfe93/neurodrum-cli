/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include <array>
#include <onnxruntime_cxx_api.h>
#include "cxxopts.hpp"

//==============================================================================
int main (int argc, char* argv[])
{
    //DBG(ORT_API_VERSION);
    
    cxxopts::Options options("neurodrum", "generate percussion samples using neural networks");
    options.add_options()
    ("m,model_file", "Model file path", cxxopts::value<std::string>()->default_value("log_kicks_full.onnx"))
    ("a,attack", "Envelope attack value 0 -> 1", cxxopts::value<float>()->default_value("0.1"))
    ("r,release", "Envelope release value 0 -> 1", cxxopts::value<float>()->default_value("0.9"))
    ("x,brightness", "Brightness value 0 -> 1", cxxopts::value<float>()->default_value("0.46533436"))
    ("y,hardness", "Hardness value 0 -> 1", cxxopts::value<float>()->default_value("0.6132435"))
    ("d,depth", "Depth value 0 -> 1", cxxopts::value<float>()->default_value("0.6906892"))
    ("z,roughness", "Roughness value 0 -> 1", cxxopts::value<float>()->default_value("0.5227648"))
    ("b,boominess", "Boominess value 0 -> 1", cxxopts::value<float>()->default_value("0.6955591"))
    ("w,warmth", "Warmth value 0 -> 1", cxxopts::value<float>()->default_value("0.733622"))
    ("s,sharpness", "Sharpness value 0 -> 1", cxxopts::value<float>()->default_value("0.4321724"))
    ("o,output", "Output directory path", cxxopts::value<std::string>()->default_value("./output"))
    ("h,help", "Print usage");
    
    auto opts = options.parse(argc, argv);
    if (opts.count("help") || !opts.count("model_file")) {
        std::cout << options.help() << std::endl;
        return 0;
    }
    auto model_file = opts["model_file"].as<std::string>();
    auto output_dir = opts["output"].as<std::string>();
    auto attackVal = opts["attack"].as<float>();
    auto releaseVal = opts["release"].as<float>();
    auto brightnessVal = opts["brightness"].as<float>();
    auto hardnessVal = opts["hardness"].as<float>();
    auto depthVal = opts["depth"].as<float>();
    auto roughnessVal = opts["roughness"].as<float>();
    auto boominessVal = opts["boominess"].as<float>();
    auto warmthVal = opts["warmth"].as<float>();
    auto sharpnessVal = opts["sharpness"].as<float>();
    
    File outputdir(output_dir);
    if (!outputdir.exists() || !outputdir.isDirectory()) { // Check if src folder exists
        outputdir.createDirectory(); // create output folder
    }
    
    const Ort::SessionOptions options_ort{nullptr};
    Ort::Env env;
    Ort::Session session_{env, model_file.c_str(), options_ort};

    Ort::Value env_tensor_{nullptr};
    std::array<int64_t, 3> env_shape_{16, 16000, 1};
    
    Ort::Value params_tensor_{nullptr};
    std::array<int64_t, 2> params_shape_{16, 7};
    
    Ort::Value is_train_tensor_{nullptr};
    std::array<int64_t, 1> is_train_shape_{1};

    Ort::Value output_tensor_{nullptr};
    std::array<int64_t, 2> output_shape_{16, 16000};
    
    
    std::array<float, 16 * 16000> input_env{};
    std::array<float, 16 * 7> input_params{};
    std::array<bool, 1> input_is_train{};
    std::array<float, 16 * 16000> results_{};
    std::vector<Ort::Value> input_tensors;
    
    /* ['brightness', 'hardness', 'depth', 'roughness', 'boominess', 'warmth', 'sharpness'] */
    const std::array<float, 7> param_vals { brightnessVal,
                                    hardnessVal,
                                    depthVal,
                                    roughnessVal,
                                    boominessVal,
                                    warmthVal,
                                    sharpnessVal };
    
    /* populate input params vector with 16 repeats of the param values */
    int counter_params = 0;
    while (counter_params < input_params.size()) {
        for (int i = 0; i < param_vals.size(); ++i) {
            input_params[counter_params + i] = param_vals[i];
        }
        counter_params += param_vals.size();
    }
    
    /* populate envelope vector with 16 repeats of the set attack/release envelope */
    const int size = 16000;
    const float endVal = 1.f;
    float currentVal = 0.f;
    int counter_env = 0;
    float stepAttack = endVal / (attackVal*size);
    float stepRelease = endVal / (releaseVal*size);
    
    std::fill(input_is_train.begin(), input_is_train.end(), false);
    std::fill(input_env.begin(), input_env.end(), 0.f);
    
    while (counter_env < input_env.size()) {
        for (int i = 0; i < size; ++i)
        {
            if (i < attackVal*size) {
                currentVal += stepAttack;
                input_env[counter_env + i] = std::min(currentVal, 1.f);;
            } else {
                currentVal -= stepRelease;
                input_env[counter_env + i] = std::max(currentVal, 0.f);
            }
        }
        currentVal = 0.f;
        counter_env += size;
    }

    

    
    auto memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);
    env_tensor_ = Ort::Value::CreateTensor<float>(memory_info, input_env.data(), input_env.size(), env_shape_.data(), env_shape_.size());
    
    params_tensor_ = Ort::Value::CreateTensor<float>(memory_info, input_params.data(), input_params.size(), params_shape_.data(), params_shape_.size());
    
    is_train_tensor_ = Ort::Value::CreateTensor<bool>(memory_info, input_is_train.data(), input_is_train.size(), is_train_shape_.data(), is_train_shape_.size());
    
    output_tensor_ = Ort::Value::CreateTensor<float>(memory_info, results_.data(), results_.size(), output_shape_.data(), output_shape_.size());
    
    
    const char* input_names[] = {"cond_placeholder:0", "input_placeholder:0", "is_train:0"};
    const char* output_names[] = {"Squeeze:0"};
    
    input_tensors.push_back(std::move(params_tensor_));
    input_tensors.push_back(std::move(env_tensor_));
    input_tensors.push_back(std::move(is_train_tensor_));

    session_.Run(Ort::RunOptions{nullptr}, input_names, input_tensors.data(), 3, output_names, &output_tensor_, 1);

    
    
    std::unique_ptr<WavAudioFormat> audioFormat = std::make_unique<WavAudioFormat>();
    
    AudioSampleBuffer outputWav;
    outputWav.setSize(2, size);
    for (int c {0}; c < outputWav.getNumChannels(); ++c)
    {
        for (int i {0}; i < size; ++i)
        {
            outputWav.setSample(c, i, results_[i]);
        }
    }

    /* render result to wav file */
    File outputFile(outputdir.getFullPathName() + "/neurodrum_sample.wav");
    FileOutputStream* outputTo;
    outputTo = outputFile.createOutputStream();
    const double fs = 16000.0;
    AudioFormatWriter* writer;
    writer = audioFormat->createWriterFor(outputTo, fs, 2, 16, NULL, 0);
    writer->writeFromAudioSampleBuffer(outputWav, 0, outputWav.getNumSamples());
    writer->~AudioFormatWriter();

    return 0;
}
