#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>


#define M_PI 3.1415926
#ifndef TONE_H
#define TONE_H

class Tone
{
private:
	const double sampleRate;
	double s; /* current position along the unit sine curve */
	double lastSample;
public:
	double getSample() const;
	double nextSample(double frequency, double amplitude);
	Tone(double sampleRate);
};

#endif


Tone::Tone(double sampleRate) : sampleRate(sampleRate), s(0), lastSample(0)
{
}

double Tone::getSample() const
{
	return this->lastSample;
}

double Tone::nextSample(double frequency, double amplitude)
{
	this->s += frequency / this->sampleRate;
	this->lastSample = amplitude * sin(2 * M_PI * this->s);
	return this->lastSample;
}






void write_sample(std::ostream& stream, double sample);

void note()
{
	constexpr double SAMPLE_RATE = 44100;
	std::vector<Tone> tones;

	// Each of these represents a final octave difference from A4
	const double octaves[] = { -2, 0, 4. / 12, 7. / 12, 1, 16. / 12, 19. / 12 };
	const size_t count = sizeof(octaves) / sizeof(*octaves);

	std::ofstream outfile("out", std::ofstream::binary);

	// Create a tone generator for each
	for (size_t i = 0; i < count; i++)
	{
		tones.push_back(Tone(SAMPLE_RATE));
	}

	// Set parameters for the output sound
	double duration = 4.0;
	double attack = 1.0;
	double decay = 2.5;
	double amplitude = 0.8;

	// Generate the required samples
	for (double t = 0; t < duration; t += 1 / SAMPLE_RATE)
	{
		// Interpolate with sinusoidal easing up to the attack time, then constant
		double interp = t >= attack ? 1 : sin(M_PI / 2 * t);

		// Constant until the decay time, then interpolate linearly out
		double strength = t < (duration - decay) ? 1
			: 1 - (t - (duration - decay)) / decay;

		// Superimpose the samples from each tone generator
		double totalSample = 0;
		for (size_t i = 0; i < count; i++)
		{
			double octave = interp * octaves[i];
			double frequency = 440 * pow(2, octave);
			double sample = tones[i].nextSample(frequency, strength * amplitude);
			totalSample += sample;
		}
		totalSample /= count;

		if (outfile.good())
		{
			write_sample(outfile, totalSample);
		}
	}

}

// Writes a sample to the given output stream as a 16-bit signed integer.
void write_sample(std::ostream& stream, double sample)
{
	int16_t output = (int16_t)(0x7FFF * sample);
	stream.write((char*)&output, sizeof(int16_t) / sizeof(char));
}