/**
*  @file tea5767.h
*
*  Created on: 23.12.2013
*  Copyright (c) 2013 by Anton Gusev aka AHTOXA
**/

#ifndef TEA5767_H_INCLUDED
#define TEA5767_H_INCLUDED

#include "soft_i2c.h"

class SampleTea5767Props
{
};

struct Tea5767TxData
{
	union
	{
		uint8_t bytes[5];
		struct
		{
			// byte 5
			uint32_t		         :6;     //
			uint32_t		DTC      :1;     // Standby (1=standby)
			uint32_t		PLLREF   :1;     // 6.5 MHz reference frequency for the PLL (1=enabled)

			// byte 4
			uint32_t		SI       :1;     // Search indicator (1=SWP1 is used as ready flag)
			uint32_t		SNC      :1;     // Stereo Noise Canceling (1=on)
			uint32_t		HCC      :1;     // High cut control (1=on)
			uint32_t		SMUTE    :1;     // Soft mute (1=on)
			uint32_t		XTAL     :1;     // 1 = 32KHz, 0 = 13MHz
			uint32_t		BL       :1;     // Band Limits (1=Japanese, 2=US/Europe)
			uint32_t		STBY     :1;     // Standby (1=standby)
			uint32_t		SWP2     :1;     // Software programmable port 2

			// byte 3
			uint32_t		SWP1     :1;     // Software programmable port 1
			uint32_t		ML       :1;     // Mute left
			uint32_t		MR       :1;     // Mute right
			uint32_t		MS       :1;     // Mono to Stereo (1=mono)
			uint32_t		HLSI     :1;     // HIGH/LOW Side Injection (1=High)
			uint32_t		SSL      :2;     // Search Stop Level (0=not allowed, 1=low, 2=med, 3=high)
			uint32_t		SUD      :1;     // Search Up/Down (1=up)

			// byte 1&2
			uint32_t		PLL      :14;    // PLL
			uint32_t		SM       :1;     // Search mode (1=in search mode)
			uint32_t		MUTE     :1;     // Mute (1=on)
		}__attribute__ ((packed))
		bits;
	};
};

struct Tea5767RxData
{
	union
	{
		uint8_t bytes[5];
		struct
		{
			// byte 5
			uint32_t		         :8;

			// byte 4
			uint32_t		         :1;
			uint32_t		CI       :3;     // chip ID (=0)
			uint32_t		LEV      :4;     // level ADC output

			// byte 3
			uint32_t		IF       :7;     // IF counter result
			uint32_t		STEREO   :1;     // stereo flag

			// byte 1&2
			uint32_t		PLL      :14;    // PLL
			uint32_t		BLF      :1;     // Band limit flag
			uint32_t		RF       :1;     // Ready flag
		}__attribute__ ((packed))
		bits;
	};
};


template <typename props = SampleTea5767Props>
class Tea5767
{
public:
	Tea5767()
		: i2c_()
	{
		for (int i = 0; i < 5; i++)
			txData_.bytes[i] = 0;
		txData_.bits.XTAL = 1; // 32KHz
		txData_.bits.HCC = 1;
		txData_.bits.SNC = 1;
		setFreq(104500000);
	}
	void readData();
	void writeData();
	void setMute(bool value) { txData_.bits.MUTE = value; };
	void setHLSI(bool value) { txData_.bits.HLSI = value; };
	void setSMUTE(bool value) { txData_.bits.SMUTE = value; };
	void setSNC(bool value) { txData_.bits.SNC = value; };
	void setHCC(bool value) { txData_.bits.HCC = value; };
	void setDTC(bool value) { txData_.bits.DTC = value; };
	uint32_t freq2pll(uint32_t freq) { return 4 * (freq - 225000) / 32768; }
	uint32_t pll2freq(uint32_t pll) { return pll * 32768 / 4 + 225000; }
	void setFreq(uint32_t freq) { txData_.bits.PLL = freq2pll(freq); }
	uint32_t getFreq() { return pll2freq(rxData_.bits.PLL); }
private:
	enum
	{
		TUNER_ADDR = 0xC0
	};
	struct SoftI2cProps
	{
		typedef Pin<'B', 10> SCL;
		typedef Pin<'B', 11> SDA;
		enum
		{
			NOP_COUNT = 100
		};
	};

	typedef SoftI2c<SoftI2cProps> I2cType;
	I2cType i2c_;
	Tea5767TxData txData_;
	Tea5767RxData rxData_;
};

template <typename props>
void Tea5767<props>::readData()
{
	i2c_.start();
	i2c_.write(TUNER_ADDR | 1);
	rxData_.bytes[4] = i2c_.read(false);
	rxData_.bytes[3] = i2c_.read(false);
	rxData_.bytes[2] = i2c_.read(false);
	rxData_.bytes[1] = i2c_.read(false);
	rxData_.bytes[0] = i2c_.read(true);
	i2c_.stop();
}

template <typename props>
void Tea5767<props>::writeData()
{
	i2c_.start();
	i2c_.write(TUNER_ADDR);
	i2c_.write(txData_.bytes[4]);
	i2c_.write(txData_.bytes[3]);
	i2c_.write(txData_.bytes[2]);
	i2c_.write(txData_.bytes[1]);
	i2c_.write(txData_.bytes[0]);
	i2c_.stop();
}

#endif // TEA5767_H_INCLUDED
