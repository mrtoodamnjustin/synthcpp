#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using std::sin;

class Ut
{
public:
  const double tau = 6.283185;

  static void writeAsBytes(std::ofstream &file, int value, int byte_size)
  {
    file.write(reinterpret_cast<const char *>(&value), byte_size);
  }
};
Ut ut;

class MetaData
{
public:
  // RIFF Chunk
  const std::string chunkId = "RIFF";
  const std::string chunkSize = "----"; // *** This must be written after the audio is written
  const std::string format = "WAVE";

  // FMT sub-chunk
  const std::string subChunk1Id = "fmt ";
  const int subChunk1Size = 16;
  const int audioFormat = 1; // PCM Format
  const int numChannels = 2; // Stereo
  const int sampleRate = 44100;
  const int byteRate = sampleRate * numChannels * (subChunk1Size / 8);
  const int blockAlign = numChannels * (subChunk1Size / 8);
  const int bitsPerSample = 16;

  // Data sub-chunk
  const std::string subChunk2Id = "data";
  const std::string subChunk2Size = "----"; // *** This must be written after the audio is written
  double duration = 6;
  const int maxAmplitude = 32760;

  std::vector<double> frequencies;

  // Writes meta data before audio and returns audio start position
  int writePreMetaData(std::ofstream &file)
  {
    if (file.is_open())
    {
      // Writes all Meta Data
      file << chunkId;
      file << chunkSize;
      file << format;

      file << subChunk1Id;
      Ut::writeAsBytes(file, subChunk1Size, 4);
      Ut::writeAsBytes(file, audioFormat, 2);
      Ut::writeAsBytes(file, numChannels, 2);
      Ut::writeAsBytes(file, sampleRate, 4);
      Ut::writeAsBytes(file, byteRate, 4);
      Ut::writeAsBytes(file, blockAlign, 2);
      Ut::writeAsBytes(file, bitsPerSample, 2);

      file << subChunk2Id;
      file << subChunk2Size;

      int startAudioPos = (int)file.tellp();
      return startAudioPos;
    }
    else
      std::cerr << "Fatal Error 1: Tried to access unopened file";
    return 1;
  }

  static void writePostMetaData(std::ofstream &file, int startAudioPos)
  {
    if (file.is_open())
    {
      // Writes last bit of meta data
      int end_audio = (int)file.tellp();
      file.seekp(startAudioPos - 4); // Sub-chunk 2 size
      Ut::writeAsBytes(file, end_audio - startAudioPos, 4);

      file.seekp(4, std::ios::beg);
      Ut::writeAsBytes(file, end_audio - 8, 4);
    }
  }
};
MetaData metaData;

class PadData
{
public:
  std::string waveType = "sin";
  double attackTime = 1;
  //    double decayTime = 1;
  //    double sustainTime = 1;
  double releaseTime = 1;

  std::vector<double> lfoFrequencies = {0};
  std::vector<double> lfoDepths = {0};

  std::vector<double> detunes = {0};

  std::vector<double> frequencies{440};

  double getSin(double time)
  {
    return sin(ut.tau * time * frequencies[0]);
  }
};
PadData padData;

class Lfo
{
public:
  std::vector<double> lfoFrequencies = {0.5};
  double depth = 0.5; // Between 0 and 1

  double lfo1(double wave, double time)
  {
    return wave * (sin(ut.tau * time * lfoFrequencies[0]) * depth) + wave * (1 - depth);
  }
};
Lfo lfo;

void WriteAudioData(std::ofstream &file);
double GetAmp(double time);

int main()
{
  std::ofstream file;
  file.open("wavFile.wav", std::ios::binary);
  int startAudioPos = metaData.writePreMetaData(file);
  WriteAudioData(file);
  MetaData::writePostMetaData(file, startAudioPos);
}

void WriteAudioData(std::ofstream &file)
{
  for (int frame = 0; frame <= metaData.sampleRate * metaData.duration; frame++)
  {
    double time = (double)frame / metaData.sampleRate;
    double amp = GetAmp(time);
    //        double wave = lfo.lfo1(padData.getSin(time), time);
    double wave = padData.getSin(time);

    Ut::writeAsBytes(file, (int)(amp * wave), 2);
  }
}

double GetAmp(double time)
{
  double maxAmplitude = metaData.maxAmplitude / 2.0;
  double currentAmplitude = maxAmplitude;

  // Attacking
  if (time < padData.attackTime)
    currentAmplitude = maxAmplitude * time / padData.attackTime;

  //    // Decaying
  //    else if (time < padData.attackTime + padData.decayTime)
  //        currentAmplitude = maxAmplitude; // ??
  //
  //    // Sustaining
  //    else if (time < padData.attackTime + padData.decayTime + padData.sustainTime)
  //        currentAmplitude = maxAmplitude; // ??

  // Releasing
  else if (time > metaData.duration - padData.releaseTime)
    currentAmplitude = -maxAmplitude / padData.releaseTime * (time - metaData.duration + padData.releaseTime) + maxAmplitude;

  if (currentAmplitude > maxAmplitude)
    std::cerr << "Non-fatal Error 2: Maximum Amplitude Exceeded";

  return currentAmplitude;
}
