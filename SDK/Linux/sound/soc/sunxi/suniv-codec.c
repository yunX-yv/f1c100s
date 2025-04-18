/*
 * Copyright 2014 Emilio López <emilio@elopez.com.ar>
 * Copyright 2014 Jon Smirl <jonsmirl@gmail.com>
 * Copyright 2015 Maxime Ripard <maxime.ripard@free-electrons.com>
 * Copyright 2015 Adam Sampson <ats@offog.org>
 * Copyright 2016 Chen-Yu Tsai <wens@csie.org>
 * Copyright 2018 Mesih Kilinc <***@gmail.com>
 *
 * Based on the Allwinner SDK driver, released under the GPL.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
#include <linux/clk.h>
#include <linux/regmap.h>
#include <linux/reset.h>
#include <linux/gpio/consumer.h>

#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/tlv.h>
#include <sound/initval.h>
#include <sound/dmaengine_pcm.h>

/* Codec DAC digital controls and FIFO registers */
#define SUN4I_CODEC_DAC_DPC			(0x00)
#define SUN4I_CODEC_DAC_DPC_EN_DA			(31)
#define SUN4I_CODEC_DAC_DPC_DVOL			(12)
#define SUN4I_CODEC_DAC_FIFOC			(0x04)
#define SUN4I_CODEC_DAC_FIFOC_DAC_FS			(29)
#define SUN4I_CODEC_DAC_FIFOC_FIR_VERSION		(28)
#define SUN4I_CODEC_DAC_FIFOC_SEND_LASAT		(26)
#define SUN4I_CODEC_DAC_FIFOC_TX_FIFO_MODE		(24)
#define SUN4I_CODEC_DAC_FIFOC_DRQ_CLR_CNT		(21)
#define SUN4I_CODEC_DAC_FIFOC_TX_TRIG_LEVEL		(8)
#define SUN4I_CODEC_DAC_FIFOC_MONO_EN			(6)
#define SUN4I_CODEC_DAC_FIFOC_TX_SAMPLE_BITS		(5)
#define SUN4I_CODEC_DAC_FIFOC_DAC_DRQ_EN		(4)
#define SUN4I_CODEC_DAC_FIFOC_FIFO_FLUSH		(0)
#define SUN4I_CODEC_DAC_FIFOS			(0x08)
#define SUN4I_CODEC_DAC_TXDATA			(0x0c)

/* Codec DAC side analog signal controls */
#define SUN4I_CODEC_DAC_ACTL			(0x10)
#define SUN4I_CODEC_DAC_ACTL_DACAENR			(31)
#define SUN4I_CODEC_DAC_ACTL_DACAENL			(30)
#define SUN4I_CODEC_DAC_ACTL_MIXEN			(29)
#define SUN4I_CODEC_DAC_ACTL_LDACLMIXS			(15)
#define SUN4I_CODEC_DAC_ACTL_RDACRMIXS			(14)
#define SUN4I_CODEC_DAC_ACTL_LDACRMIXS			(13)
#define SUN4I_CODEC_DAC_ACTL_DACPAS			(8)
#define SUN4I_CODEC_DAC_ACTL_MIXPAS			(7)
#define SUN4I_CODEC_DAC_ACTL_PA_MUTE			(6)
#define SUN4I_CODEC_DAC_ACTL_PA_VOL			(0)
#define SUN4I_CODEC_DAC_TUNE			(0x14)
#define SUN4I_CODEC_DAC_DEBUG			(0x18)

/* Codec ADC digital controls and FIFO registers */
#define SUN4I_CODEC_ADC_FIFOC			(0x1c)
#define SUN4I_CODEC_ADC_FIFOC_ADC_FS			(29)
#define SUN4I_CODEC_ADC_FIFOC_EN_AD			(28)
#define SUN4I_CODEC_ADC_FIFOC_RX_FIFO_MODE		(24)
#define SUN4I_CODEC_ADC_FIFOC_RX_TRIG_LEVEL		(8)
#define SUN4I_CODEC_ADC_FIFOC_MONO_EN			(7)
#define SUN4I_CODEC_ADC_FIFOC_RX_SAMPLE_BITS		(6)
#define SUN4I_CODEC_ADC_FIFOC_ADC_DRQ_EN		(4)
#define SUN4I_CODEC_ADC_FIFOC_FIFO_FLUSH		(0)
#define SUN4I_CODEC_ADC_FIFOS			(0x20)
#define SUN4I_CODEC_ADC_RXDATA			(0x24)

/* Codec ADC side analog signal controls */
#define SUN4I_CODEC_ADC_ACTL			(0x28)
#define SUN4I_CODEC_ADC_ACTL_ADC_R_EN			(31)
#define SUN4I_CODEC_ADC_ACTL_ADC_L_EN			(30)
#define SUN4I_CODEC_ADC_ACTL_PREG1EN			(29)
#define SUN4I_CODEC_ADC_ACTL_PREG2EN			(28)
#define SUN4I_CODEC_ADC_ACTL_VMICEN			(27)
#define SUN4I_CODEC_ADC_ACTL_VADCG			(20)
#define SUN4I_CODEC_ADC_ACTL_ADCIS			(17)
#define SUN4I_CODEC_ADC_ACTL_PA_EN			(4)
#define SUN4I_CODEC_ADC_ACTL_DDE			(3)
#define SUN4I_CODEC_ADC_DEBUG			(0x2c)

/* FIFO counters */
#define SUN4I_CODEC_DAC_TXCNT			(0x30)
#define SUN4I_CODEC_ADC_RXCNT			(0x34)

/* Calibration register (sun7i only) */
#define SUN7I_CODEC_AC_DAC_CAL			(0x38)

/* Microphone controls (sun7i only) */
#define SUN7I_CODEC_AC_MIC_PHONE_CAL		(0x3c)

/*
 * sun6i specific registers
 *
 * sun6i shares the same digital control and FIFO registers as sun4i,
 * but only the DAC digital controls are at the same offset. The others
 * have been moved around to accommodate extra analog controls.
 */

/* Codec DAC digital controls and FIFO registers */
#define SUN6I_CODEC_ADC_FIFOC			(0x10)
#define SUN6I_CODEC_ADC_FIFOC_EN_AD			(28)
#define SUN6I_CODEC_ADC_FIFOS			(0x14)
#define SUN6I_CODEC_ADC_RXDATA			(0x18)

/* Output mixer and gain controls */
#define SUN6I_CODEC_OM_DACA_CTRL		(0x20)
#define SUN6I_CODEC_OM_DACA_CTRL_DACAREN		(31)
#define SUN6I_CODEC_OM_DACA_CTRL_DACALEN		(30)
#define SUN6I_CODEC_OM_DACA_CTRL_RMIXEN			(29)
#define SUN6I_CODEC_OM_DACA_CTRL_LMIXEN			(28)
#define SUN6I_CODEC_OM_DACA_CTRL_RMIX_MIC1		(23)
#define SUN6I_CODEC_OM_DACA_CTRL_RMIX_MIC2		(22)
#define SUN6I_CODEC_OM_DACA_CTRL_RMIX_PHONE		(21)
#define SUN6I_CODEC_OM_DACA_CTRL_RMIX_PHONEP		(20)
#define SUN6I_CODEC_OM_DACA_CTRL_RMIX_LINEINR		(19)
#define SUN6I_CODEC_OM_DACA_CTRL_RMIX_DACR		(18)
#define SUN6I_CODEC_OM_DACA_CTRL_RMIX_DACL		(17)
#define SUN6I_CODEC_OM_DACA_CTRL_LMIX_MIC1		(16)
#define SUN6I_CODEC_OM_DACA_CTRL_LMIX_MIC2		(15)
#define SUN6I_CODEC_OM_DACA_CTRL_LMIX_PHONE		(14)
#define SUN6I_CODEC_OM_DACA_CTRL_LMIX_PHONEN		(13)
#define SUN6I_CODEC_OM_DACA_CTRL_LMIX_LINEINL		(12)
#define SUN6I_CODEC_OM_DACA_CTRL_LMIX_DACL		(11)
#define SUN6I_CODEC_OM_DACA_CTRL_LMIX_DACR		(10)
#define SUN6I_CODEC_OM_DACA_CTRL_RHPIS			(9)
#define SUN6I_CODEC_OM_DACA_CTRL_LHPIS			(8)
#define SUN6I_CODEC_OM_DACA_CTRL_RHPPAMUTE		(7)
#define SUN6I_CODEC_OM_DACA_CTRL_LHPPAMUTE		(6)
#define SUN6I_CODEC_OM_DACA_CTRL_HPVOL			(0)
#define SUN6I_CODEC_OM_PA_CTRL			(0x24)
#define SUN6I_CODEC_OM_PA_CTRL_HPPAEN			(31)
#define SUN6I_CODEC_OM_PA_CTRL_HPCOM_CTL		(29)
#define SUN6I_CODEC_OM_PA_CTRL_COMPTEN			(28)
#define SUN6I_CODEC_OM_PA_CTRL_MIC1G			(15)
#define SUN6I_CODEC_OM_PA_CTRL_MIC2G			(12)
#define SUN6I_CODEC_OM_PA_CTRL_LINEING			(9)
#define SUN6I_CODEC_OM_PA_CTRL_PHONEG			(6)
#define SUN6I_CODEC_OM_PA_CTRL_PHONEPG			(3)
#define SUN6I_CODEC_OM_PA_CTRL_PHONENG			(0)

/* Microphone, line out and phone out controls */
#define SUN6I_CODEC_MIC_CTRL			(0x28)
#define SUN6I_CODEC_MIC_CTRL_HBIASEN			(31)
#define SUN6I_CODEC_MIC_CTRL_MBIASEN			(30)
#define SUN6I_CODEC_MIC_CTRL_MIC1AMPEN			(28)
#define SUN6I_CODEC_MIC_CTRL_MIC1BOOST			(25)
#define SUN6I_CODEC_MIC_CTRL_MIC2AMPEN			(24)
#define SUN6I_CODEC_MIC_CTRL_MIC2BOOST			(21)
#define SUN6I_CODEC_MIC_CTRL_MIC2SLT			(20)
#define SUN6I_CODEC_MIC_CTRL_LINEOUTLEN			(19)
#define SUN6I_CODEC_MIC_CTRL_LINEOUTREN			(18)
#define SUN6I_CODEC_MIC_CTRL_LINEOUTLSRC		(17)
#define SUN6I_CODEC_MIC_CTRL_LINEOUTRSRC		(16)
#define SUN6I_CODEC_MIC_CTRL_LINEOUTVC			(11)
#define SUN6I_CODEC_MIC_CTRL_PHONEPREG			(8)

/* ADC mixer controls */
#define SUN6I_CODEC_ADC_ACTL			(0x2c)
#define SUN6I_CODEC_ADC_ACTL_ADCREN			(31)
#define SUN6I_CODEC_ADC_ACTL_ADCLEN			(30)
#define SUN6I_CODEC_ADC_ACTL_ADCRG			(27)
#define SUN6I_CODEC_ADC_ACTL_ADCLG			(24)
#define SUN6I_CODEC_ADC_ACTL_RADCMIX_MIC1		(13)
#define SUN6I_CODEC_ADC_ACTL_RADCMIX_MIC2		(12)
#define SUN6I_CODEC_ADC_ACTL_RADCMIX_PHONE		(11)
#define SUN6I_CODEC_ADC_ACTL_RADCMIX_PHONEP		(10)
#define SUN6I_CODEC_ADC_ACTL_RADCMIX_LINEINR		(9)
#define SUN6I_CODEC_ADC_ACTL_RADCMIX_OMIXR		(8)
#define SUN6I_CODEC_ADC_ACTL_RADCMIX_OMIXL		(7)
#define SUN6I_CODEC_ADC_ACTL_LADCMIX_MIC1		(6)
#define SUN6I_CODEC_ADC_ACTL_LADCMIX_MIC2		(5)
#define SUN6I_CODEC_ADC_ACTL_LADCMIX_PHONE		(4)
#define SUN6I_CODEC_ADC_ACTL_LADCMIX_PHONEN		(3)
#define SUN6I_CODEC_ADC_ACTL_LADCMIX_LINEINL		(2)
#define SUN6I_CODEC_ADC_ACTL_LADCMIX_OMIXL		(1)
#define SUN6I_CODEC_ADC_ACTL_LADCMIX_OMIXR		(0)

/* Analog performance tuning controls */
#define SUN6I_CODEC_ADDA_TUNE			(0x30)

/* Calibration controls */
#define SUN6I_CODEC_CALIBRATION			(0x34)

/* FIFO counters */
#define SUN6I_CODEC_DAC_TXCNT			(0x40)
#define SUN6I_CODEC_ADC_RXCNT			(0x44)

/* headset jack detection and button support registers */
#define SUN6I_CODEC_HMIC_CTL			(0x50)
#define SUN6I_CODEC_HMIC_DATA			(0x54)

/* TODO sun6i DAP (Digital Audio Processing) bits */

/* FIFO counters moved on A23 */
#define SUN8I_A23_CODEC_DAC_TXCNT		(0x1c)
#define SUN8I_A23_CODEC_ADC_RXCNT		(0x20)

/* TX FIFO moved on H3 */
#define SUN8I_H3_CODEC_DAC_TXDATA		(0x20)
#define SUN8I_H3_CODEC_DAC_DBG			(0x48)
#define SUN8I_H3_CODEC_ADC_DBG			(0x4c)

/* TODO H3 DAP (Digital Audio Processing) bits */

#define SUN4I_DMA_MAX_BURST				(8)

/* suniv specific registers */

#define SUNIV_DMA_MAX_BURST					(4)

/* Codec DAC digital controls and FIFO registers */
#define SUNIV_CODEC_ADC_FIFOC 				(0x10)
#define SUNIV_CODEC_ADC_FIFOC_EN_AD			(28)
#define SUNIV_CODEC_ADC_FIFOS				(0x14)
#define SUNIV_CODEC_ADC_RXDATA				(0x18)

/* Output mixer and gain controls */
#define SUNIV_CODEC_OM_DACA_CTRL 			(0x20)
#define SUNIV_CODEC_OM_DACA_CTRL_DACAREN	(31)
#define SUNIV_CODEC_OM_DACA_CTRL_DACALEN	(30)
#define SUNIV_CODEC_OM_DACA_CTRL_RMIXEN 	(29)
#define SUNIV_CODEC_OM_DACA_CTRL_LMIXEN 	(28)
#define SUNIV_CODEC_OM_DACA_CTRL_RHPPAMUTE	(27)
#define SUNIV_CODEC_OM_DACA_CTRL_LHPPAMUTE	(26)
#define SUNIV_CODEC_OM_DACA_CTRL_RHPIS		(25)
#define SUNIV_CODEC_OM_DACA_CTRL_LHPIS		(24)
#define SUNIV_CODEC_OM_DACA_CTRL_HPCOM_CTL	(22)
#define SUNIV_CODEC_OM_DACA_CTRL_COMPTEN	(21)
#define SUNIV_CODEC_OM_DACA_CTRL_RMIXMUTE_MICIN		(20)
#define SUNIV_CODEC_OM_DACA_CTRL_RMIXMUTE_LINEIN	(19)
#define SUNIV_CODEC_OM_DACA_CTRL_RMIXMUTE_FMIN		(18)
#define SUNIV_CODEC_OM_DACA_CTRL_RMIXMUTE_RDAC		(17)
#define SUNIV_CODEC_OM_DACA_CTRL_RMIXMUTE_LDAC		(16)
#define SUNIV_CODEC_OM_DACA_CTRL_HPPAEN		(15)
#define SUNIV_CODEC_OM_DACA_CTRL_LMIXMUTE_MICIN		(12)
#define SUNIV_CODEC_OM_DACA_CTRL_LMIXMUTE_LINEIN	(11)
#define SUNIV_CODEC_OM_DACA_CTRL_LMIXMUTE_FMIN		(10)
#define SUNIV_CODEC_OM_DACA_CTRL_LMIXMUTE_LDAC		(9)
#define SUNIV_CODEC_OM_DACA_CTRL_LMIXMUTE_RDAC		(8)
#define SUNIV_CODEC_OM_DACA_CTRL_LTLNMUTE	(7)
#define SUNIV_CODEC_OM_DACA_CTRL_RTLNMUTE	(6)
#define SUNIV_CODEC_OM_DACA_CTRL_HPVOL		(0)

/* Analog Input Mixer controls */
#define SUNIV_CODEC_ADC_ACTL 				(0x24)
#define SUNIV_CODEC_ADC_ADCEN 				(31)
#define SUNIV_CODEC_ADC_MICG				(24)
#define SUNIV_CODEC_ADC_LINEINVOL			(21)
#define SUNIV_CODEC_ADC_ADCG				(16)
#define SUNIV_CODEC_ADC_ADCMIX_MIC			(13)
#define SUNIV_CODEC_ADC_ADCMIX_FMINL		(12)
#define SUNIV_CODEC_ADC_ADCMIX_FMINR		(11)
#define SUNIV_CODEC_ADC_ADCMIX_LINEIN		(10)
#define SUNIV_CODEC_ADC_ADCMIX_LOUT			(9)
#define SUNIV_CODEC_ADC_ADCMIX_ROUT			(8)
#define SUNIV_CODEC_ADC_PASPEEDSELECT		(7)
#define SUNIV_CODEC_ADC_FMINVOL				(4)
#define SUNIV_CODEC_ADC_MICAMPEN			(3)
#define SUNIV_CODEC_ADC_MICBOOST			(0)

#define SUNIV_CODEC_ADC_DBG					(0x4c)

struct sun4i_codec {
	struct device	*dev;
	struct regmap	*regmap;
	struct clk	*clk_apb;
	struct clk	*clk_module;
	struct reset_control *rst;
	struct gpio_desc *gpio_pa;

	/* ADC_FIFOC register is at different offset on different SoCs */
	struct regmap_field *reg_adc_fifoc;

	struct snd_dmaengine_dai_dma_data	capture_dma_data;
	struct snd_dmaengine_dai_dma_data	playback_dma_data;
};

static void sun4i_codec_start_playback(struct sun4i_codec *scodec)
{
	/* Flush TX FIFO */
	regmap_update_bits(scodec->regmap, SUN4I_CODEC_DAC_FIFOC,
			   BIT(SUN4I_CODEC_DAC_FIFOC_FIFO_FLUSH),
			   BIT(SUN4I_CODEC_DAC_FIFOC_FIFO_FLUSH));

	/* Enable DAC DRQ */
	regmap_update_bits(scodec->regmap, SUN4I_CODEC_DAC_FIFOC,
			   BIT(SUN4I_CODEC_DAC_FIFOC_DAC_DRQ_EN),
			   BIT(SUN4I_CODEC_DAC_FIFOC_DAC_DRQ_EN));
}

static void sun4i_codec_stop_playback(struct sun4i_codec *scodec)
{
	/* Disable DAC DRQ */
	regmap_update_bits(scodec->regmap, SUN4I_CODEC_DAC_FIFOC,
			   BIT(SUN4I_CODEC_DAC_FIFOC_DAC_DRQ_EN),
			   0);
}

static void sun4i_codec_start_capture(struct sun4i_codec *scodec)
{
	/* Enable ADC DRQ */
	regmap_field_update_bits(scodec->reg_adc_fifoc,
				 BIT(SUN4I_CODEC_ADC_FIFOC_ADC_DRQ_EN),
				 BIT(SUN4I_CODEC_ADC_FIFOC_ADC_DRQ_EN));
}

static void sun4i_codec_stop_capture(struct sun4i_codec *scodec)
{
	/* Disable ADC DRQ */
	regmap_field_update_bits(scodec->reg_adc_fifoc,
				 BIT(SUN4I_CODEC_ADC_FIFOC_ADC_DRQ_EN), 0);
}

static int sun4i_codec_trigger(struct snd_pcm_substream *substream, int cmd,
			       struct snd_soc_dai *dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct sun4i_codec *scodec = snd_soc_card_get_drvdata(rtd->card);

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
			sun4i_codec_start_playback(scodec);
		else
			sun4i_codec_start_capture(scodec);
		break;

	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
			sun4i_codec_stop_playback(scodec);
		else
			sun4i_codec_stop_capture(scodec);
		break;

	default:
		return -EINVAL;
	}

	return 0;
}

static int sun4i_codec_prepare_capture(struct snd_pcm_substream *substream,
				       struct snd_soc_dai *dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct sun4i_codec *scodec = snd_soc_card_get_drvdata(rtd->card);


	/* Flush RX FIFO */
	regmap_field_update_bits(scodec->reg_adc_fifoc,
				 BIT(SUN4I_CODEC_ADC_FIFOC_FIFO_FLUSH),
				 BIT(SUN4I_CODEC_ADC_FIFOC_FIFO_FLUSH));


	/* Set RX FIFO trigger level */
	regmap_field_update_bits(scodec->reg_adc_fifoc,
				 0xf << SUN4I_CODEC_ADC_FIFOC_RX_TRIG_LEVEL,
				 0x7 << SUN4I_CODEC_ADC_FIFOC_RX_TRIG_LEVEL);

	/*
	 * FIXME: Undocumented in the datasheet, but
	 *        Allwinner's code mentions that it is related
	 *        related to microphone gain
	 */
	if (of_device_is_compatible(scodec->dev->of_node,
				    "allwinner,sun4i-a10-codec") ||
	    of_device_is_compatible(scodec->dev->of_node,
				    "allwinner,sun7i-a20-codec")) {
		regmap_update_bits(scodec->regmap, SUN4I_CODEC_ADC_ACTL,
				   0x3 << 25,
				   0x1 << 25);
	}

	if (of_device_is_compatible(scodec->dev->of_node,
				    "allwinner,sun7i-a20-codec"))
		/* FIXME: Undocumented bits */
		regmap_update_bits(scodec->regmap, SUN4I_CODEC_DAC_TUNE,
				   0x3 << 8,
				   0x1 << 8);

	return 0;
}

static int sun4i_codec_prepare_playback(struct snd_pcm_substream *substream,
					struct snd_soc_dai *dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct sun4i_codec *scodec = snd_soc_card_get_drvdata(rtd->card);
	u32 val;

	/* Flush the TX FIFO */
	regmap_update_bits(scodec->regmap, SUN4I_CODEC_DAC_FIFOC,
			   BIT(SUN4I_CODEC_DAC_FIFOC_FIFO_FLUSH),
			   BIT(SUN4I_CODEC_DAC_FIFOC_FIFO_FLUSH));

	/* Set TX FIFO Empty Trigger Level */
	regmap_update_bits(scodec->regmap, SUN4I_CODEC_DAC_FIFOC,
			   0x3f << SUN4I_CODEC_DAC_FIFOC_TX_TRIG_LEVEL,
			   0xf << SUN4I_CODEC_DAC_FIFOC_TX_TRIG_LEVEL);

	if (substream->runtime->rate > 32000)
		/* Use 64 bits FIR filter */
		val = 0;
	else
		/* Use 32 bits FIR filter */
		val = BIT(SUN4I_CODEC_DAC_FIFOC_FIR_VERSION);

	regmap_update_bits(scodec->regmap, SUN4I_CODEC_DAC_FIFOC,
			   BIT(SUN4I_CODEC_DAC_FIFOC_FIR_VERSION),
			   val);

	/* Send zeros when we have an underrun */
	regmap_update_bits(scodec->regmap, SUN4I_CODEC_DAC_FIFOC,
			   BIT(SUN4I_CODEC_DAC_FIFOC_SEND_LASAT),
			   0);

	return 0;
};

static int sun4i_codec_prepare(struct snd_pcm_substream *substream,
			       struct snd_soc_dai *dai)
{
	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		return sun4i_codec_prepare_playback(substream, dai);

	return sun4i_codec_prepare_capture(substream, dai);
}

static unsigned long sun4i_codec_get_mod_freq(struct snd_pcm_hw_params *params)
{
	unsigned int rate = params_rate(params);

	switch (rate) {
	case 176400:
	case 88200:
	case 44100:
	case 33075:
	case 22050:
	case 14700:
	case 11025:
	case 7350:
		return 22579200;

	case 192000:
	case 96000:
	case 48000:
	case 32000:
	case 24000:
	case 16000:
	case 12000:
	case 8000:
		return 24576000;

	default:
		return 0;
	}
}

static int sun4i_codec_get_hw_rate(struct snd_pcm_hw_params *params)
{
	unsigned int rate = params_rate(params);

	switch (rate) {
	case 192000:
	case 176400:
		return 6;

	case 96000:
	case 88200:
		return 7;

	case 48000:
	case 44100:
		return 0;

	case 32000:
	case 33075:
		return 1;

	case 24000:
	case 22050:
		return 2;

	case 16000:
	case 14700:
		return 3;

	case 12000:
	case 11025:
		return 4;

	case 8000:
	case 7350:
		return 5;

	default:
		return -EINVAL;
	}
}

static int sun4i_codec_hw_params_capture(struct sun4i_codec *scodec,
					 struct snd_pcm_hw_params *params,
					 unsigned int hwrate)
{
	/* Set ADC sample rate */
	regmap_field_update_bits(scodec->reg_adc_fifoc,
				 7 << SUN4I_CODEC_ADC_FIFOC_ADC_FS,
				 hwrate << SUN4I_CODEC_ADC_FIFOC_ADC_FS);

	/* Set the number of channels we want to use */
	if (params_channels(params) == 1)
		regmap_field_update_bits(scodec->reg_adc_fifoc,
					 BIT(SUN4I_CODEC_ADC_FIFOC_MONO_EN),
					 BIT(SUN4I_CODEC_ADC_FIFOC_MONO_EN));
	else
		regmap_field_update_bits(scodec->reg_adc_fifoc,
					 BIT(SUN4I_CODEC_ADC_FIFOC_MONO_EN),
					 0);

	/* Set the number of sample bits to either 16 or 24 bits */
	if (hw_param_interval(params, SNDRV_PCM_HW_PARAM_SAMPLE_BITS)->min == 32) {
		regmap_field_update_bits(scodec->reg_adc_fifoc,
				   BIT(SUN4I_CODEC_ADC_FIFOC_RX_SAMPLE_BITS),
				   BIT(SUN4I_CODEC_ADC_FIFOC_RX_SAMPLE_BITS));

		regmap_field_update_bits(scodec->reg_adc_fifoc,
				   BIT(SUN4I_CODEC_ADC_FIFOC_RX_FIFO_MODE),
				   0);

		scodec->capture_dma_data.addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	} else {
		regmap_field_update_bits(scodec->reg_adc_fifoc,
				   BIT(SUN4I_CODEC_ADC_FIFOC_RX_SAMPLE_BITS),
				   0);

		/* Fill most significant bits with valid data MSB */
		regmap_field_update_bits(scodec->reg_adc_fifoc,
				   BIT(SUN4I_CODEC_ADC_FIFOC_RX_FIFO_MODE),
				   BIT(SUN4I_CODEC_ADC_FIFOC_RX_FIFO_MODE));

		scodec->capture_dma_data.addr_width = DMA_SLAVE_BUSWIDTH_2_BYTES;
	}

	return 0;
}

static int sun4i_codec_hw_params_playback(struct sun4i_codec *scodec,
					  struct snd_pcm_hw_params *params,
					  unsigned int hwrate)
{
	u32 val;

	/* Set DAC sample rate */
	regmap_update_bits(scodec->regmap, SUN4I_CODEC_DAC_FIFOC,
			   7 << SUN4I_CODEC_DAC_FIFOC_DAC_FS,
			   hwrate << SUN4I_CODEC_DAC_FIFOC_DAC_FS);

	/* Set the number of channels we want to use */
	if (params_channels(params) == 1)
		val = BIT(SUN4I_CODEC_DAC_FIFOC_MONO_EN);
	else
		val = 0;

	regmap_update_bits(scodec->regmap, SUN4I_CODEC_DAC_FIFOC,
			   BIT(SUN4I_CODEC_DAC_FIFOC_MONO_EN),
			   val);

	/* Set the number of sample bits to either 16 or 24 bits */
	if (hw_param_interval(params, SNDRV_PCM_HW_PARAM_SAMPLE_BITS)->min == 32) {
		regmap_update_bits(scodec->regmap, SUN4I_CODEC_DAC_FIFOC,
				   BIT(SUN4I_CODEC_DAC_FIFOC_TX_SAMPLE_BITS),
				   BIT(SUN4I_CODEC_DAC_FIFOC_TX_SAMPLE_BITS));

		/* Set TX FIFO mode to padding the LSBs with 0 */
		regmap_update_bits(scodec->regmap, SUN4I_CODEC_DAC_FIFOC,
				   BIT(SUN4I_CODEC_DAC_FIFOC_TX_FIFO_MODE),
				   0);

		scodec->playback_dma_data.addr_width = DMA_SLAVE_BUSWIDTH_4_BYTES;
	} else {
		regmap_update_bits(scodec->regmap, SUN4I_CODEC_DAC_FIFOC,
				   BIT(SUN4I_CODEC_DAC_FIFOC_TX_SAMPLE_BITS),
				   0);

		/* Set TX FIFO mode to repeat the MSB */
		regmap_update_bits(scodec->regmap, SUN4I_CODEC_DAC_FIFOC,
				   BIT(SUN4I_CODEC_DAC_FIFOC_TX_FIFO_MODE),
				   BIT(SUN4I_CODEC_DAC_FIFOC_TX_FIFO_MODE));

		scodec->playback_dma_data.addr_width = DMA_SLAVE_BUSWIDTH_2_BYTES;
	}

	return 0;
}

static int sun4i_codec_hw_params(struct snd_pcm_substream *substream,
				 struct snd_pcm_hw_params *params,
				 struct snd_soc_dai *dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct sun4i_codec *scodec = snd_soc_card_get_drvdata(rtd->card);
	unsigned long clk_freq;
	int ret, hwrate;

	clk_freq = sun4i_codec_get_mod_freq(params);
	if (!clk_freq)
		return -EINVAL;

	ret = clk_set_rate(scodec->clk_module, clk_freq);
	if (ret)
		return ret;

	hwrate = sun4i_codec_get_hw_rate(params);
	if (hwrate < 0)
		return hwrate;

	if (substream->stream == SNDRV_PCM_STREAM_PLAYBACK)
		return sun4i_codec_hw_params_playback(scodec, params,
						      hwrate);

	return sun4i_codec_hw_params_capture(scodec, params,
					     hwrate);
}


static unsigned int sun4i_codec_src_rates[] = {
	8000, 11025, 12000, 16000, 22050, 24000, 32000,
	44100, 48000, 96000, 192000
};


static struct snd_pcm_hw_constraint_list sun4i_codec_constraints = {
	.count  = ARRAY_SIZE(sun4i_codec_src_rates),
	.list   = sun4i_codec_src_rates,
};


static int sun4i_codec_startup(struct snd_pcm_substream *substream,
			       struct snd_soc_dai *dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct sun4i_codec *scodec = snd_soc_card_get_drvdata(rtd->card);

	snd_pcm_hw_constraint_list(substream->runtime, 0,
				SNDRV_PCM_HW_PARAM_RATE, &sun4i_codec_constraints);

	/*
	 * Stop issuing DRQ when we have room for less than 16 samples
	 * in our TX FIFO
	 */
	regmap_update_bits(scodec->regmap, SUN4I_CODEC_DAC_FIFOC,
			   3 << SUN4I_CODEC_DAC_FIFOC_DRQ_CLR_CNT,
			   3 << SUN4I_CODEC_DAC_FIFOC_DRQ_CLR_CNT);

	return clk_prepare_enable(scodec->clk_module);
}

static void sun4i_codec_shutdown(struct snd_pcm_substream *substream,
				 struct snd_soc_dai *dai)
{
	struct snd_soc_pcm_runtime *rtd = substream->private_data;
	struct sun4i_codec *scodec = snd_soc_card_get_drvdata(rtd->card);

	clk_disable_unprepare(scodec->clk_module);
}

static const struct snd_soc_dai_ops sun4i_codec_dai_ops = {
	.startup	= sun4i_codec_startup,
	.shutdown	= sun4i_codec_shutdown,
	.trigger	= sun4i_codec_trigger,
	.hw_params	= sun4i_codec_hw_params,
	.prepare	= sun4i_codec_prepare,
};

static struct snd_soc_dai_driver sun4i_codec_dai = {
	.name	= "Codec",
	.ops	= &sun4i_codec_dai_ops,
	.playback = {
		.stream_name	= "Codec Playback",
		.channels_min	= 1,
		.channels_max	= 2,
		.rate_min	= 8000,
		.rate_max	= 192000,
		.rates		= SNDRV_PCM_RATE_CONTINUOUS,
		.formats	= SNDRV_PCM_FMTBIT_S16_LE |
				  SNDRV_PCM_FMTBIT_S32_LE,
		.sig_bits	= 24,
	},
	.capture = {
		.stream_name	= "Codec Capture",
		.channels_min	= 1,
		.channels_max	= 2,
		.rate_min	= 8000,
		.rate_max	= 48000,
		.rates		= SNDRV_PCM_RATE_CONTINUOUS,
		.formats	= SNDRV_PCM_FMTBIT_S16_LE |
				  SNDRV_PCM_FMTBIT_S32_LE,
		.sig_bits	= 24,
	},
};

/*** sun4i Codec ***/
static const struct snd_kcontrol_new sun4i_codec_pa_mute =
	SOC_DAPM_SINGLE("Switch", SUN4I_CODEC_DAC_ACTL,
			SUN4I_CODEC_DAC_ACTL_PA_MUTE, 1, 0);

static DECLARE_TLV_DB_SCALE(sun4i_codec_pa_volume_scale, -6300, 100, 1);

static const struct snd_kcontrol_new sun4i_codec_controls[] = {
	SOC_SINGLE_TLV("Power Amplifier Volume", SUN4I_CODEC_DAC_ACTL,
		       SUN4I_CODEC_DAC_ACTL_PA_VOL, 0x3F, 0,
		       sun4i_codec_pa_volume_scale),
};

static const struct snd_kcontrol_new sun4i_codec_left_mixer_controls[] = {
	SOC_DAPM_SINGLE("Left DAC Playback Switch", SUN4I_CODEC_DAC_ACTL,
			SUN4I_CODEC_DAC_ACTL_LDACLMIXS, 1, 0),
};

static const struct snd_kcontrol_new sun4i_codec_right_mixer_controls[] = {
	SOC_DAPM_SINGLE("Right DAC Playback Switch", SUN4I_CODEC_DAC_ACTL,
			SUN4I_CODEC_DAC_ACTL_RDACRMIXS, 1, 0),
	SOC_DAPM_SINGLE("Left DAC Playback Switch", SUN4I_CODEC_DAC_ACTL,
			SUN4I_CODEC_DAC_ACTL_LDACRMIXS, 1, 0),
};

static const struct snd_kcontrol_new sun4i_codec_pa_mixer_controls[] = {
	SOC_DAPM_SINGLE("DAC Playback Switch", SUN4I_CODEC_DAC_ACTL,
			SUN4I_CODEC_DAC_ACTL_DACPAS, 1, 0),
	SOC_DAPM_SINGLE("Mixer Playback Switch", SUN4I_CODEC_DAC_ACTL,
			SUN4I_CODEC_DAC_ACTL_MIXPAS, 1, 0),
};

static const struct snd_soc_dapm_widget sun4i_codec_codec_dapm_widgets[] = {
	/* Digital parts of the ADCs */
	SND_SOC_DAPM_SUPPLY("ADC", SUN4I_CODEC_ADC_FIFOC,
			    SUN4I_CODEC_ADC_FIFOC_EN_AD, 0,
			    NULL, 0),

	/* Digital parts of the DACs */
	SND_SOC_DAPM_SUPPLY("DAC", SUN4I_CODEC_DAC_DPC,
			    SUN4I_CODEC_DAC_DPC_EN_DA, 0,
			    NULL, 0),

	/* Analog parts of the ADCs */
	SND_SOC_DAPM_ADC("Left ADC", "Codec Capture", SUN4I_CODEC_ADC_ACTL,
			 SUN4I_CODEC_ADC_ACTL_ADC_L_EN, 0),
	SND_SOC_DAPM_ADC("Right ADC", "Codec Capture", SUN4I_CODEC_ADC_ACTL,
			 SUN4I_CODEC_ADC_ACTL_ADC_R_EN, 0),

	/* Analog parts of the DACs */
	SND_SOC_DAPM_DAC("Left DAC", "Codec Playback", SUN4I_CODEC_DAC_ACTL,
			 SUN4I_CODEC_DAC_ACTL_DACAENL, 0),
	SND_SOC_DAPM_DAC("Right DAC", "Codec Playback", SUN4I_CODEC_DAC_ACTL,
			 SUN4I_CODEC_DAC_ACTL_DACAENR, 0),

	/* Mixers */
	SND_SOC_DAPM_MIXER("Left Mixer", SND_SOC_NOPM, 0, 0,
			   sun4i_codec_left_mixer_controls,
			   ARRAY_SIZE(sun4i_codec_left_mixer_controls)),
	SND_SOC_DAPM_MIXER("Right Mixer", SND_SOC_NOPM, 0, 0,
			   sun4i_codec_right_mixer_controls,
			   ARRAY_SIZE(sun4i_codec_right_mixer_controls)),

	/* Global Mixer Enable */
	SND_SOC_DAPM_SUPPLY("Mixer Enable", SUN4I_CODEC_DAC_ACTL,
			    SUN4I_CODEC_DAC_ACTL_MIXEN, 0, NULL, 0),

	/* VMIC */
	SND_SOC_DAPM_SUPPLY("VMIC", SUN4I_CODEC_ADC_ACTL,
			    SUN4I_CODEC_ADC_ACTL_VMICEN, 0, NULL, 0),

	/* Mic Pre-Amplifiers */
	SND_SOC_DAPM_PGA("MIC1 Pre-Amplifier", SUN4I_CODEC_ADC_ACTL,
			 SUN4I_CODEC_ADC_ACTL_PREG1EN, 0, NULL, 0),

	/* Power Amplifier */
	SND_SOC_DAPM_MIXER("Power Amplifier", SUN4I_CODEC_ADC_ACTL,
			   SUN4I_CODEC_ADC_ACTL_PA_EN, 0,
			   sun4i_codec_pa_mixer_controls,
			   ARRAY_SIZE(sun4i_codec_pa_mixer_controls)),
	SND_SOC_DAPM_SWITCH("Power Amplifier Mute", SND_SOC_NOPM, 0, 0,
			    &sun4i_codec_pa_mute),

	SND_SOC_DAPM_INPUT("Mic1"),

	SND_SOC_DAPM_OUTPUT("HP Right"),
	SND_SOC_DAPM_OUTPUT("HP Left"),
};

static const struct snd_soc_dapm_route sun4i_codec_codec_dapm_routes[] = {
	/* Left ADC / DAC Routes */
	{ "Left ADC", NULL, "ADC" },
	{ "Left DAC", NULL, "DAC" },

	/* Right ADC / DAC Routes */
	{ "Right ADC", NULL, "ADC" },
	{ "Right DAC", NULL, "DAC" },

	/* Right Mixer Routes */
	{ "Right Mixer", NULL, "Mixer Enable" },
	{ "Right Mixer", "Left DAC Playback Switch", "Left DAC" },
	{ "Right Mixer", "Right DAC Playback Switch", "Right DAC" },

	/* Left Mixer Routes */
	{ "Left Mixer", NULL, "Mixer Enable" },
	{ "Left Mixer", "Left DAC Playback Switch", "Left DAC" },

	/* Power Amplifier Routes */
	{ "Power Amplifier", "Mixer Playback Switch", "Left Mixer" },
	{ "Power Amplifier", "Mixer Playback Switch", "Right Mixer" },
	{ "Power Amplifier", "DAC Playback Switch", "Left DAC" },
	{ "Power Amplifier", "DAC Playback Switch", "Right DAC" },

	/* Headphone Output Routes */
	{ "Power Amplifier Mute", "Switch", "Power Amplifier" },
	{ "HP Right", NULL, "Power Amplifier Mute" },
	{ "HP Left", NULL, "Power Amplifier Mute" },

	/* Mic1 Routes */
	{ "Left ADC", NULL, "MIC1 Pre-Amplifier" },
	{ "Right ADC", NULL, "MIC1 Pre-Amplifier" },
	{ "MIC1 Pre-Amplifier", NULL, "Mic1"},
	{ "Mic1", NULL, "VMIC" },
};

static const struct snd_soc_component_driver sun4i_codec_codec = {
	.controls		= sun4i_codec_controls,
	.num_controls		= ARRAY_SIZE(sun4i_codec_controls),
	.dapm_widgets		= sun4i_codec_codec_dapm_widgets,
	.num_dapm_widgets	= ARRAY_SIZE(sun4i_codec_codec_dapm_widgets),
	.dapm_routes		= sun4i_codec_codec_dapm_routes,
	.num_dapm_routes	= ARRAY_SIZE(sun4i_codec_codec_dapm_routes),
	.idle_bias_on		= 1,
	.pmdown_time	= 1,
	.endianness		= 1,
	.non_legacy_dai_naming	= 1,
};

/*** sun6i Codec ***/

/* mixer controls */
static const struct snd_kcontrol_new sun6i_codec_mixer_controls[] = {
	SOC_DAPM_DOUBLE("DAC Playback Switch",
			SUN6I_CODEC_OM_DACA_CTRL,
			SUN6I_CODEC_OM_DACA_CTRL_LMIX_DACL,
			SUN6I_CODEC_OM_DACA_CTRL_RMIX_DACR, 1, 0),
	SOC_DAPM_DOUBLE("DAC Reversed Playback Switch",
			SUN6I_CODEC_OM_DACA_CTRL,
			SUN6I_CODEC_OM_DACA_CTRL_LMIX_DACR,
			SUN6I_CODEC_OM_DACA_CTRL_RMIX_DACL, 1, 0),
	SOC_DAPM_DOUBLE("Line In Playback Switch",
			SUN6I_CODEC_OM_DACA_CTRL,
			SUN6I_CODEC_OM_DACA_CTRL_LMIX_LINEINL,
			SUN6I_CODEC_OM_DACA_CTRL_RMIX_LINEINR, 1, 0),
	SOC_DAPM_DOUBLE("Mic1 Playback Switch",
			SUN6I_CODEC_OM_DACA_CTRL,
			SUN6I_CODEC_OM_DACA_CTRL_LMIX_MIC1,
			SUN6I_CODEC_OM_DACA_CTRL_RMIX_MIC1, 1, 0),
	SOC_DAPM_DOUBLE("Mic2 Playback Switch",
			SUN6I_CODEC_OM_DACA_CTRL,
			SUN6I_CODEC_OM_DACA_CTRL_LMIX_MIC2,
			SUN6I_CODEC_OM_DACA_CTRL_RMIX_MIC2, 1, 0),
};

/* ADC mixer controls */
static const struct snd_kcontrol_new sun6i_codec_adc_mixer_controls[] = {
	SOC_DAPM_DOUBLE("Mixer Capture Switch",
			SUN6I_CODEC_ADC_ACTL,
			SUN6I_CODEC_ADC_ACTL_LADCMIX_OMIXL,
			SUN6I_CODEC_ADC_ACTL_RADCMIX_OMIXR, 1, 0),
	SOC_DAPM_DOUBLE("Mixer Reversed Capture Switch",
			SUN6I_CODEC_ADC_ACTL,
			SUN6I_CODEC_ADC_ACTL_LADCMIX_OMIXR,
			SUN6I_CODEC_ADC_ACTL_RADCMIX_OMIXL, 1, 0),
	SOC_DAPM_DOUBLE("Line In Capture Switch",
			SUN6I_CODEC_ADC_ACTL,
			SUN6I_CODEC_ADC_ACTL_LADCMIX_LINEINL,
			SUN6I_CODEC_ADC_ACTL_RADCMIX_LINEINR, 1, 0),
	SOC_DAPM_DOUBLE("Mic1 Capture Switch",
			SUN6I_CODEC_ADC_ACTL,
			SUN6I_CODEC_ADC_ACTL_LADCMIX_MIC1,
			SUN6I_CODEC_ADC_ACTL_RADCMIX_MIC1, 1, 0),
	SOC_DAPM_DOUBLE("Mic2 Capture Switch",
			SUN6I_CODEC_ADC_ACTL,
			SUN6I_CODEC_ADC_ACTL_LADCMIX_MIC2,
			SUN6I_CODEC_ADC_ACTL_RADCMIX_MIC2, 1, 0),
};

/* headphone controls */
static const char * const sun6i_codec_hp_src_enum_text[] = {
	"DAC", "Mixer",
};

static SOC_ENUM_DOUBLE_DECL(sun6i_codec_hp_src_enum,
			    SUN6I_CODEC_OM_DACA_CTRL,
			    SUN6I_CODEC_OM_DACA_CTRL_LHPIS,
			    SUN6I_CODEC_OM_DACA_CTRL_RHPIS,
			    sun6i_codec_hp_src_enum_text);

static const struct snd_kcontrol_new sun6i_codec_hp_src[] = {
	SOC_DAPM_ENUM("Headphone Source Playback Route",
		      sun6i_codec_hp_src_enum),
};

/* microphone controls */
static const char * const sun6i_codec_mic2_src_enum_text[] = {
	"Mic2", "Mic3",
};

static SOC_ENUM_SINGLE_DECL(sun6i_codec_mic2_src_enum,
			    SUN6I_CODEC_MIC_CTRL,
			    SUN6I_CODEC_MIC_CTRL_MIC2SLT,
			    sun6i_codec_mic2_src_enum_text);

static const struct snd_kcontrol_new sun6i_codec_mic2_src[] = {
	SOC_DAPM_ENUM("Mic2 Amplifier Source Route",
		      sun6i_codec_mic2_src_enum),
};

/* line out controls */
static const char * const sun6i_codec_lineout_src_enum_text[] = {
	"Stereo", "Mono Differential",
};

static SOC_ENUM_DOUBLE_DECL(sun6i_codec_lineout_src_enum,
			    SUN6I_CODEC_MIC_CTRL,
			    SUN6I_CODEC_MIC_CTRL_LINEOUTLSRC,
			    SUN6I_CODEC_MIC_CTRL_LINEOUTRSRC,
			    sun6i_codec_lineout_src_enum_text);

static const struct snd_kcontrol_new sun6i_codec_lineout_src[] = {
	SOC_DAPM_ENUM("Line Out Source Playback Route",
		      sun6i_codec_lineout_src_enum),
};

/* volume / mute controls */
static const DECLARE_TLV_DB_SCALE(sun6i_codec_dvol_scale, -7308, 116, 0);
static const DECLARE_TLV_DB_SCALE(sun6i_codec_hp_vol_scale, -6300, 100, 1);
static const DECLARE_TLV_DB_SCALE(sun6i_codec_out_mixer_pregain_scale,
				  -450, 150, 0);
static const DECLARE_TLV_DB_RANGE(sun6i_codec_lineout_vol_scale,
	0, 1, TLV_DB_SCALE_ITEM(TLV_DB_GAIN_MUTE, 0, 1),
	2, 31, TLV_DB_SCALE_ITEM(-4350, 150, 0),
);
static const DECLARE_TLV_DB_RANGE(sun6i_codec_mic_gain_scale,
	0, 0, TLV_DB_SCALE_ITEM(0, 0, 0),
	1, 7, TLV_DB_SCALE_ITEM(2400, 300, 0),
);

static const struct snd_kcontrol_new sun6i_codec_codec_widgets[] = {
	SOC_SINGLE_TLV("DAC Playback Volume", SUN4I_CODEC_DAC_DPC,
		       SUN4I_CODEC_DAC_DPC_DVOL, 0x3f, 1,
		       sun6i_codec_dvol_scale),
	SOC_SINGLE_TLV("Headphone Playback Volume",
		       SUN6I_CODEC_OM_DACA_CTRL,
		       SUN6I_CODEC_OM_DACA_CTRL_HPVOL, 0x3f, 0,
		       sun6i_codec_hp_vol_scale),
	SOC_SINGLE_TLV("Line Out Playback Volume",
		       SUN6I_CODEC_MIC_CTRL,
		       SUN6I_CODEC_MIC_CTRL_LINEOUTVC, 0x1f, 0,
		       sun6i_codec_lineout_vol_scale),
	SOC_DOUBLE("Headphone Playback Switch",
		   SUN6I_CODEC_OM_DACA_CTRL,
		   SUN6I_CODEC_OM_DACA_CTRL_LHPPAMUTE,
		   SUN6I_CODEC_OM_DACA_CTRL_RHPPAMUTE, 1, 0),
	SOC_DOUBLE("Line Out Playback Switch",
		   SUN6I_CODEC_MIC_CTRL,
		   SUN6I_CODEC_MIC_CTRL_LINEOUTLEN,
		   SUN6I_CODEC_MIC_CTRL_LINEOUTREN, 1, 0),
	/* Mixer pre-gains */
	SOC_SINGLE_TLV("Line In Playback Volume",
		       SUN6I_CODEC_OM_PA_CTRL, SUN6I_CODEC_OM_PA_CTRL_LINEING,
		       0x7, 0, sun6i_codec_out_mixer_pregain_scale),
	SOC_SINGLE_TLV("Mic1 Playback Volume",
		       SUN6I_CODEC_OM_PA_CTRL, SUN6I_CODEC_OM_PA_CTRL_MIC1G,
		       0x7, 0, sun6i_codec_out_mixer_pregain_scale),
	SOC_SINGLE_TLV("Mic2 Playback Volume",
		       SUN6I_CODEC_OM_PA_CTRL, SUN6I_CODEC_OM_PA_CTRL_MIC2G,
		       0x7, 0, sun6i_codec_out_mixer_pregain_scale),

	/* Microphone Amp boost gains */
	SOC_SINGLE_TLV("Mic1 Boost Volume", SUN6I_CODEC_MIC_CTRL,
		       SUN6I_CODEC_MIC_CTRL_MIC1BOOST, 0x7, 0,
		       sun6i_codec_mic_gain_scale),
	SOC_SINGLE_TLV("Mic2 Boost Volume", SUN6I_CODEC_MIC_CTRL,
		       SUN6I_CODEC_MIC_CTRL_MIC2BOOST, 0x7, 0,
		       sun6i_codec_mic_gain_scale),
	SOC_DOUBLE_TLV("ADC Capture Volume",
		       SUN6I_CODEC_ADC_ACTL, SUN6I_CODEC_ADC_ACTL_ADCLG,
		       SUN6I_CODEC_ADC_ACTL_ADCRG, 0x7, 0,
		       sun6i_codec_out_mixer_pregain_scale),
};

static const struct snd_soc_dapm_widget sun6i_codec_codec_dapm_widgets[] = {
	/* Microphone inputs */
	SND_SOC_DAPM_INPUT("MIC1"),
	SND_SOC_DAPM_INPUT("MIC2"),
	SND_SOC_DAPM_INPUT("MIC3"),

	/* Microphone Bias */
	SND_SOC_DAPM_SUPPLY("HBIAS", SUN6I_CODEC_MIC_CTRL,
			    SUN6I_CODEC_MIC_CTRL_HBIASEN, 0, NULL, 0),
	SND_SOC_DAPM_SUPPLY("MBIAS", SUN6I_CODEC_MIC_CTRL,
			    SUN6I_CODEC_MIC_CTRL_MBIASEN, 0, NULL, 0),

	/* Mic input path */
	SND_SOC_DAPM_MUX("Mic2 Amplifier Source Route",
			 SND_SOC_NOPM, 0, 0, sun6i_codec_mic2_src),
	SND_SOC_DAPM_PGA("Mic1 Amplifier", SUN6I_CODEC_MIC_CTRL,
			 SUN6I_CODEC_MIC_CTRL_MIC1AMPEN, 0, NULL, 0),
	SND_SOC_DAPM_PGA("Mic2 Amplifier", SUN6I_CODEC_MIC_CTRL,
			 SUN6I_CODEC_MIC_CTRL_MIC2AMPEN, 0, NULL, 0),

	/* Line In */
	SND_SOC_DAPM_INPUT("LINEIN"),

	/* Digital parts of the ADCs */
	SND_SOC_DAPM_SUPPLY("ADC Enable", SUN6I_CODEC_ADC_FIFOC,
			    SUN6I_CODEC_ADC_FIFOC_EN_AD, 0,
			    NULL, 0),

	/* Analog parts of the ADCs */
	SND_SOC_DAPM_ADC("Left ADC", "Codec Capture", SUN6I_CODEC_ADC_ACTL,
			 SUN6I_CODEC_ADC_ACTL_ADCLEN, 0),
	SND_SOC_DAPM_ADC("Right ADC", "Codec Capture", SUN6I_CODEC_ADC_ACTL,
			 SUN6I_CODEC_ADC_ACTL_ADCREN, 0),

	/* ADC Mixers */
	SOC_MIXER_ARRAY("Left ADC Mixer", SND_SOC_NOPM, 0, 0,
			sun6i_codec_adc_mixer_controls),
	SOC_MIXER_ARRAY("Right ADC Mixer", SND_SOC_NOPM, 0, 0,
			sun6i_codec_adc_mixer_controls),

	/* Digital parts of the DACs */
	SND_SOC_DAPM_SUPPLY("DAC Enable", SUN4I_CODEC_DAC_DPC,
			    SUN4I_CODEC_DAC_DPC_EN_DA, 0,
			    NULL, 0),

	/* Analog parts of the DACs */
	SND_SOC_DAPM_DAC("Left DAC", "Codec Playback",
			 SUN6I_CODEC_OM_DACA_CTRL,
			 SUN6I_CODEC_OM_DACA_CTRL_DACALEN, 0),
	SND_SOC_DAPM_DAC("Right DAC", "Codec Playback",
			 SUN6I_CODEC_OM_DACA_CTRL,
			 SUN6I_CODEC_OM_DACA_CTRL_DACAREN, 0),

	/* Mixers */
	SOC_MIXER_ARRAY("Left Mixer", SUN6I_CODEC_OM_DACA_CTRL,
			SUN6I_CODEC_OM_DACA_CTRL_LMIXEN, 0,
			sun6i_codec_mixer_controls),
	SOC_MIXER_ARRAY("Right Mixer", SUN6I_CODEC_OM_DACA_CTRL,
			SUN6I_CODEC_OM_DACA_CTRL_RMIXEN, 0,
			sun6i_codec_mixer_controls),

	/* Headphone output path */
	SND_SOC_DAPM_MUX("Headphone Source Playback Route",
			 SND_SOC_NOPM, 0, 0, sun6i_codec_hp_src),
	SND_SOC_DAPM_OUT_DRV("Headphone Amp", SUN6I_CODEC_OM_PA_CTRL,
			     SUN6I_CODEC_OM_PA_CTRL_HPPAEN, 0, NULL, 0),
	SND_SOC_DAPM_SUPPLY("HPCOM Protection", SUN6I_CODEC_OM_PA_CTRL,
			    SUN6I_CODEC_OM_PA_CTRL_COMPTEN, 0, NULL, 0),
	SND_SOC_DAPM_REG(snd_soc_dapm_supply, "HPCOM", SUN6I_CODEC_OM_PA_CTRL,
			 SUN6I_CODEC_OM_PA_CTRL_HPCOM_CTL, 0x3, 0x3, 0),
	SND_SOC_DAPM_OUTPUT("HP"),

	/* Line Out path */
	SND_SOC_DAPM_MUX("Line Out Source Playback Route",
			 SND_SOC_NOPM, 0, 0, sun6i_codec_lineout_src),
	SND_SOC_DAPM_OUTPUT("LINEOUT"),
};

static const struct snd_soc_dapm_route sun6i_codec_codec_dapm_routes[] = {
	/* DAC Routes */
	{ "Left DAC", NULL, "DAC Enable" },
	{ "Right DAC", NULL, "DAC Enable" },

	/* Microphone Routes */
	{ "Mic1 Amplifier", NULL, "MIC1"},
	{ "Mic2 Amplifier Source Route", "Mic2", "MIC2" },
	{ "Mic2 Amplifier Source Route", "Mic3", "MIC3" },
	{ "Mic2 Amplifier", NULL, "Mic2 Amplifier Source Route"},

	/* Left Mixer Routes */
	{ "Left Mixer", "DAC Playback Switch", "Left DAC" },
	{ "Left Mixer", "DAC Reversed Playback Switch", "Right DAC" },
	{ "Left Mixer", "Line In Playback Switch", "LINEIN" },
	{ "Left Mixer", "Mic1 Playback Switch", "Mic1 Amplifier" },
	{ "Left Mixer", "Mic2 Playback Switch", "Mic2 Amplifier" },

	/* Right Mixer Routes */
	{ "Right Mixer", "DAC Playback Switch", "Right DAC" },
	{ "Right Mixer", "DAC Reversed Playback Switch", "Left DAC" },
	{ "Right Mixer", "Line In Playback Switch", "LINEIN" },
	{ "Right Mixer", "Mic1 Playback Switch", "Mic1 Amplifier" },
	{ "Right Mixer", "Mic2 Playback Switch", "Mic2 Amplifier" },

	/* Left ADC Mixer Routes */
	{ "Left ADC Mixer", "Mixer Capture Switch", "Left Mixer" },
	{ "Left ADC Mixer", "Mixer Reversed Capture Switch", "Right Mixer" },
	{ "Left ADC Mixer", "Line In Capture Switch", "LINEIN" },
	{ "Left ADC Mixer", "Mic1 Capture Switch", "Mic1 Amplifier" },
	{ "Left ADC Mixer", "Mic2 Capture Switch", "Mic2 Amplifier" },

	/* Right ADC Mixer Routes */
	{ "Right ADC Mixer", "Mixer Capture Switch", "Right Mixer" },
	{ "Right ADC Mixer", "Mixer Reversed Capture Switch", "Left Mixer" },
	{ "Right ADC Mixer", "Line In Capture Switch", "LINEIN" },
	{ "Right ADC Mixer", "Mic1 Capture Switch", "Mic1 Amplifier" },
	{ "Right ADC Mixer", "Mic2 Capture Switch", "Mic2 Amplifier" },

	/* Headphone Routes */
	{ "Headphone Source Playback Route", "DAC", "Left DAC" },
	{ "Headphone Source Playback Route", "DAC", "Right DAC" },
	{ "Headphone Source Playback Route", "Mixer", "Left Mixer" },
	{ "Headphone Source Playback Route", "Mixer", "Right Mixer" },
	{ "Headphone Amp", NULL, "Headphone Source Playback Route" },
	{ "HP", NULL, "Headphone Amp" },
	{ "HPCOM", NULL, "HPCOM Protection" },

	/* Line Out Routes */
	{ "Line Out Source Playback Route", "Stereo", "Left Mixer" },
	{ "Line Out Source Playback Route", "Stereo", "Right Mixer" },
	{ "Line Out Source Playback Route", "Mono Differential", "Left Mixer" },
	{ "Line Out Source Playback Route", "Mono Differential", "Right Mixer" },
	{ "LINEOUT", NULL, "Line Out Source Playback Route" },

	/* ADC Routes */
	{ "Left ADC", NULL, "ADC Enable" },
	{ "Right ADC", NULL, "ADC Enable" },
	{ "Left ADC", NULL, "Left ADC Mixer" },
	{ "Right ADC", NULL, "Right ADC Mixer" },
};

static const struct snd_soc_component_driver sun6i_codec_codec = {
	.controls		= sun6i_codec_codec_widgets,
	.num_controls		= ARRAY_SIZE(sun6i_codec_codec_widgets),
	.dapm_widgets		= sun6i_codec_codec_dapm_widgets,
	.num_dapm_widgets	= ARRAY_SIZE(sun6i_codec_codec_dapm_widgets),
	.dapm_routes		= sun6i_codec_codec_dapm_routes,
	.num_dapm_routes	= ARRAY_SIZE(sun6i_codec_codec_dapm_routes),
	.idle_bias_on		= 1,
	.pmdown_time	= 1,
	.endianness		= 1,
	.non_legacy_dai_naming	= 1,
};

/* sun8i A23 codec */
static const struct snd_kcontrol_new sun8i_a23_codec_codec_controls[] = {
	SOC_SINGLE_TLV("DAC Playback Volume", SUN4I_CODEC_DAC_DPC,
		       SUN4I_CODEC_DAC_DPC_DVOL, 0x3f, 1,
		       sun6i_codec_dvol_scale),
};

static const struct snd_soc_dapm_widget sun8i_a23_codec_codec_widgets[] = {
	/* Digital parts of the ADCs */
	SND_SOC_DAPM_SUPPLY("ADC Enable", SUN6I_CODEC_ADC_FIFOC,
			    SUN6I_CODEC_ADC_FIFOC_EN_AD, 0, NULL, 0),
	/* Digital parts of the DACs */
	SND_SOC_DAPM_SUPPLY("DAC Enable", SUN4I_CODEC_DAC_DPC,
			    SUN4I_CODEC_DAC_DPC_EN_DA, 0, NULL, 0),

};

static const struct snd_soc_component_driver sun8i_a23_codec_codec = {
	.controls		= sun8i_a23_codec_codec_controls,
	.num_controls		= ARRAY_SIZE(sun8i_a23_codec_codec_controls),
	.dapm_widgets		= sun8i_a23_codec_codec_widgets,
	.num_dapm_widgets	= ARRAY_SIZE(sun8i_a23_codec_codec_widgets),
	.idle_bias_on		= 1,
	.pmdown_time	= 1,
	.endianness		= 1,
	.non_legacy_dai_naming	= 1,
};

/*suniv F1C100s codec */

/* headphone controls */
static const char * const suniv_codec_hp_src_enum_text[] = {
	"DAC", "Mixer",
};

static SOC_ENUM_DOUBLE_DECL(suniv_codec_hp_src_enum,
	SUNIV_CODEC_OM_DACA_CTRL,
	SUNIV_CODEC_OM_DACA_CTRL_LHPIS,
	SUNIV_CODEC_OM_DACA_CTRL_RHPIS,
	suniv_codec_hp_src_enum_text);

static const struct snd_kcontrol_new suniv_codec_hp_src[] = {
	SOC_DAPM_ENUM("Headphone Source Playback Route",
	suniv_codec_hp_src_enum),
};


/* mixer controls */
static const struct snd_kcontrol_new suniv_codec_adc_mixer_controls[] = {
	SOC_DAPM_SINGLE("Right Out Capture Switch", SUNIV_CODEC_ADC_ACTL,
					SUNIV_CODEC_ADC_ADCMIX_ROUT, 1, 0),
	SOC_DAPM_SINGLE("Left Out Capture Switch", SUNIV_CODEC_ADC_ACTL,
					SUNIV_CODEC_ADC_ADCMIX_LOUT, 1, 0),
	SOC_DAPM_SINGLE("Line In Capture Switch", SUNIV_CODEC_ADC_ACTL,
					SUNIV_CODEC_ADC_ADCMIX_LINEIN, 1, 0),
	SOC_DAPM_SINGLE("Right FM In Capture Switch", SUNIV_CODEC_ADC_ACTL,
					SUNIV_CODEC_ADC_ADCMIX_FMINR, 1, 0),
	SOC_DAPM_SINGLE("Left FM In Capture Switch", SUNIV_CODEC_ADC_ACTL,
					SUNIV_CODEC_ADC_ADCMIX_FMINL, 1, 0),
	SOC_DAPM_SINGLE("Mic Capture Switch", SUNIV_CODEC_ADC_ACTL,
					SUNIV_CODEC_ADC_ADCMIX_MIC, 1, 0),
};

static const struct snd_kcontrol_new suniv_codec_dac_lmixer_controls[] = {
	SOC_DAPM_SINGLE("Right DAC Playback Switch", SUNIV_CODEC_OM_DACA_CTRL,
					SUNIV_CODEC_OM_DACA_CTRL_LMIXMUTE_RDAC, 1, 0),
	SOC_DAPM_SINGLE("Left DAC Playback Switch", SUNIV_CODEC_OM_DACA_CTRL,
					SUNIV_CODEC_OM_DACA_CTRL_LMIXMUTE_LDAC, 1, 0),
	SOC_DAPM_SINGLE("FM In Playback Switch", SUNIV_CODEC_OM_DACA_CTRL,
					SUNIV_CODEC_OM_DACA_CTRL_LMIXMUTE_FMIN, 1, 0),
	SOC_DAPM_SINGLE("Line In Playback Switch", SUNIV_CODEC_OM_DACA_CTRL,
					SUNIV_CODEC_OM_DACA_CTRL_LMIXMUTE_LINEIN, 1, 0),
	SOC_DAPM_SINGLE("Mic In Playback Switch", SUNIV_CODEC_OM_DACA_CTRL,
					SUNIV_CODEC_OM_DACA_CTRL_LMIXMUTE_MICIN, 1, 0),
};

static const struct snd_kcontrol_new suniv_codec_dac_rmixer_controls[] = {
	SOC_DAPM_SINGLE("Left DAC Playback Switch", SUNIV_CODEC_OM_DACA_CTRL,
					SUNIV_CODEC_OM_DACA_CTRL_RMIXMUTE_LDAC, 1, 0),
	SOC_DAPM_SINGLE("Right DAC Playback Switch", SUNIV_CODEC_OM_DACA_CTRL,
					SUNIV_CODEC_OM_DACA_CTRL_RMIXMUTE_RDAC, 1, 0),
	SOC_DAPM_SINGLE("FM In Playback Switch", SUNIV_CODEC_OM_DACA_CTRL,
					SUNIV_CODEC_OM_DACA_CTRL_RMIXMUTE_FMIN, 1, 0),
	SOC_DAPM_SINGLE("Line In Playback Switch", SUNIV_CODEC_OM_DACA_CTRL,
					SUNIV_CODEC_OM_DACA_CTRL_RMIXMUTE_LINEIN, 1, 0),
	SOC_DAPM_SINGLE("Mic In Playback Switch", SUNIV_CODEC_OM_DACA_CTRL,
					SUNIV_CODEC_OM_DACA_CTRL_RMIXMUTE_MICIN, 1, 0),
};


static const DECLARE_TLV_DB_SCALE(suniv_codec_dvol_scale, -7308, 116, 0);
static const DECLARE_TLV_DB_SCALE(suniv_codec_hp_vol_scale, -6300, 100, 1);
static const DECLARE_TLV_DB_SCALE(suniv_codec_out_mixer_pregain_scale, -450, 150, 0);


static const DECLARE_TLV_DB_RANGE(suniv_codec_mic_gain_scale,
				0, 0, TLV_DB_SCALE_ITEM(0, 0, 0),
				1, 7, TLV_DB_SCALE_ITEM(2400, 300, 0),
);


static const struct snd_kcontrol_new suniv_codec_codec_widgets[] = {
	SOC_SINGLE_TLV("DAC Playback Volume", SUN4I_CODEC_DAC_DPC,
				SUN4I_CODEC_DAC_DPC_DVOL, 0x3f, 1,
				suniv_codec_dvol_scale),
//	SOC_SINGLE_TLV("Headphone Internal Playback Volume", SUN4I_CODEC_DAC_DPC,
//				SUNIV_CODEC_OM_DACA_CTRL_HPVOL, 0x3f, 0,
//				suniv_codec_hp_vol_scale),
	SOC_SINGLE_TLV("Headphone Playback Volume",
				SUNIV_CODEC_OM_DACA_CTRL,
				SUNIV_CODEC_OM_DACA_CTRL_HPVOL, 0x3f, 0,
				suniv_codec_hp_vol_scale),
	SOC_DOUBLE("Headphone Playback Switch",
				SUNIV_CODEC_OM_DACA_CTRL,
				SUNIV_CODEC_OM_DACA_CTRL_LHPPAMUTE,
				SUNIV_CODEC_OM_DACA_CTRL_RHPPAMUTE, 1, 0),
	SOC_SINGLE_TLV("Line In Playback Volume",
				SUNIV_CODEC_ADC_ACTL, SUNIV_CODEC_ADC_LINEINVOL,
				0x7, 0, suniv_codec_out_mixer_pregain_scale),
	SOC_SINGLE_TLV("FM In Playback Volume",
				SUNIV_CODEC_ADC_ACTL, SUNIV_CODEC_ADC_FMINVOL,
				0x7, 0, suniv_codec_out_mixer_pregain_scale),
	SOC_SINGLE_TLV("Mic In Playback Volume",
				SUNIV_CODEC_ADC_ACTL, SUNIV_CODEC_ADC_MICG,
				0x7, 0, suniv_codec_out_mixer_pregain_scale),

	/* Microphone Amp boost gains */
	SOC_SINGLE_TLV("Mic Boost Volume", SUNIV_CODEC_ADC_ACTL,
				SUNIV_CODEC_ADC_MICBOOST, 0x7, 0,
				suniv_codec_mic_gain_scale),
	SOC_SINGLE_TLV("ADC Capture Volume",
				SUNIV_CODEC_ADC_ACTL, SUNIV_CODEC_ADC_ADCG,
				0x7, 0, suniv_codec_out_mixer_pregain_scale),
};

static const struct snd_soc_dapm_widget suniv_codec_codec_dapm_widgets[] = {
	/* Microphone inputs */
	SND_SOC_DAPM_INPUT("MIC"),

	/* Microphone Bias */
	/* deleted: HBIAS, MBIAS */

	/* Mic input path */
	SND_SOC_DAPM_PGA("Mic Amplifier", SUNIV_CODEC_ADC_ACTL,
				SUNIV_CODEC_ADC_MICAMPEN, 0, NULL, 0),

	/* Line In */
	SND_SOC_DAPM_INPUT("LINEIN"),

	/* FM In */
	SND_SOC_DAPM_INPUT("FMINR"),
	SND_SOC_DAPM_INPUT("FMINL"),

	/* Digital parts of the ADCs */
	SND_SOC_DAPM_SUPPLY("ADC Enable", SUNIV_CODEC_ADC_FIFOC,
				SUNIV_CODEC_ADC_FIFOC_EN_AD, 0,
				NULL, 0),

	/* Analog parts of the ADCs */
	SND_SOC_DAPM_ADC("ADC", "Codec Capture", SUNIV_CODEC_ADC_ACTL,
				SUNIV_CODEC_ADC_ADCEN, 0),

	/* ADC Mixers */
	SOC_MIXER_ARRAY("ADC Mixer", SUNIV_CODEC_ADC_ACTL,
				SND_SOC_NOPM, 0,
				suniv_codec_adc_mixer_controls),

	/* Digital parts of the DACs */
	SND_SOC_DAPM_SUPPLY("DAC Enable", SUN4I_CODEC_DAC_DPC,
				SUN4I_CODEC_DAC_DPC_EN_DA, 0,
				NULL, 0),

	/* Analog parts of the DACs */
	SND_SOC_DAPM_DAC("Left DAC", "Codec Playback",
				SUNIV_CODEC_OM_DACA_CTRL,
				SUNIV_CODEC_OM_DACA_CTRL_DACALEN, 0),
	SND_SOC_DAPM_DAC("Right DAC", "Codec Playback",
				SUNIV_CODEC_OM_DACA_CTRL,
				SUNIV_CODEC_OM_DACA_CTRL_DACAREN, 0),

	/* Mixers */
	SOC_MIXER_ARRAY("Left Mixer", SUNIV_CODEC_OM_DACA_CTRL,
				SUNIV_CODEC_OM_DACA_CTRL_LMIXEN, 0,
				suniv_codec_dac_lmixer_controls),
	SOC_MIXER_ARRAY("Right Mixer", SUNIV_CODEC_OM_DACA_CTRL,
				SUNIV_CODEC_OM_DACA_CTRL_RMIXEN, 0,
				suniv_codec_dac_rmixer_controls),

	/* Headphone output path */
	SND_SOC_DAPM_MUX("Headphone Source Playback Route",
				SND_SOC_NOPM, 0, 0, suniv_codec_hp_src),
	SND_SOC_DAPM_OUT_DRV("Headphone Amp", SUNIV_CODEC_OM_DACA_CTRL,
				SUNIV_CODEC_OM_DACA_CTRL_HPPAEN, 0, NULL, 0),
	SND_SOC_DAPM_SUPPLY("HPCOM Protection", SUNIV_CODEC_OM_DACA_CTRL,
				SUNIV_CODEC_OM_DACA_CTRL_COMPTEN, 0, NULL, 0),
	SND_SOC_DAPM_REG(snd_soc_dapm_supply, "HPCOM", SUNIV_CODEC_OM_DACA_CTRL,
				SUNIV_CODEC_OM_DACA_CTRL_HPCOM_CTL, 0x3, 0x3, 0),
	SND_SOC_DAPM_OUTPUT("HP"),
};

static const struct snd_soc_dapm_route suniv_codec_codec_dapm_routes[] = {
	/* DAC Routes */
	{ "Left DAC", NULL, "DAC Enable" },
	{ "Right DAC", NULL, "DAC Enable" },

	/* Microphone Routes */
	{ "Mic Amplifier", NULL, "MIC"},

	/* Left Mixer Routes */
	{ "Left Mixer", "Right DAC Playback Switch", "Right DAC" },
	{ "Left Mixer", "Left DAC Playback Switch", "Left DAC" },
	{ "Left Mixer", "FM In Playback Switch", "FMINL" },
	{ "Left Mixer", "Line In Playback Switch", "LINEIN" },
	{ "Left Mixer", "Mic In Playback Switch", "Mic Amplifier" },

	/* Right Mixer Routes */
	{ "Right Mixer", "Left DAC Playback Switch", "Left DAC" },
	{ "Right Mixer", "Right DAC Playback Switch", "Right DAC" },
	{ "Right Mixer", "FM In Playback Switch", "FMINR" },
	{ "Right Mixer", "Line In Playback Switch", "LINEIN" },
	{ "Right Mixer", "Mic In Playback Switch", "Mic Amplifier" },

	/* ADC Mixer Routes */
	{ "ADC Mixer", "Right Out Capture Switch", "Right Mixer" },
	{ "ADC Mixer", "Left Out Capture Switch", "Left Mixer" },
	{ "ADC Mixer", "Line In Capture Switch", "LINEIN" },
	{ "ADC Mixer", "Right FM In Capture Switch", "FMINR" },
	{ "ADC Mixer", "Left FM In Capture Switch", "FMINL" },
	{ "ADC Mixer", "Mic Capture Switch", "Mic Amplifier" },

	/* Headphone Routes */
	{ "Headphone Source Playback Route", "DAC", "Left DAC" },
	{ "Headphone Source Playback Route", "DAC", "Right DAC" },
	{ "Headphone Source Playback Route", "Mixer", "Left Mixer" },
	{ "Headphone Source Playback Route", "Mixer", "Right Mixer" },
	{ "Headphone Amp", NULL, "Headphone Source Playback Route" },
	{ "HP", NULL, "Headphone Amp" },
	{ "HPCOM", NULL, "HPCOM Protection" },

	/* ADC Routes */
	{ "ADC", NULL, "ADC Mixer" },
	{ "ADC", NULL, "ADC Enable" },
};

static const struct snd_soc_component_driver suniv_codec_codec = {
	.controls = suniv_codec_codec_widgets,
	.num_controls = ARRAY_SIZE(suniv_codec_codec_widgets),
	.dapm_widgets = suniv_codec_codec_dapm_widgets,
	.num_dapm_widgets = ARRAY_SIZE(suniv_codec_codec_dapm_widgets),
	.dapm_routes = suniv_codec_codec_dapm_routes,
	.num_dapm_routes = ARRAY_SIZE(suniv_codec_codec_dapm_routes),
	.idle_bias_on = 1,
	.pmdown_time = 1,
	.endianness = 1,
	.non_legacy_dai_naming = 1,
};


static const struct snd_soc_component_driver sun4i_codec_component = {
	.name = "sun4i-codec",
};

#define SUN4I_CODEC_RATES	SNDRV_PCM_RATE_CONTINUOUS
#define SUN4I_CODEC_FORMATS	(SNDRV_PCM_FMTBIT_S16_LE | \
				 SNDRV_PCM_FMTBIT_S32_LE)

static int sun4i_codec_dai_probe(struct snd_soc_dai *dai)
{
	struct snd_soc_card *card = snd_soc_dai_get_drvdata(dai);
	struct sun4i_codec *scodec = snd_soc_card_get_drvdata(card);

	snd_soc_dai_init_dma_data(dai, &scodec->playback_dma_data,
				  &scodec->capture_dma_data);

	return 0;
}

static struct snd_soc_dai_driver dummy_cpu_dai = {
	.name	= "sun4i-codec-cpu-dai",
	.probe	= sun4i_codec_dai_probe,
	.playback = {
		.stream_name	= "Playback",
		.channels_min	= 1,
		.channels_max	= 2,
		.rates		= SUN4I_CODEC_RATES,
		.formats	= SUN4I_CODEC_FORMATS,
		.sig_bits	= 24,
	},
	.capture = {
		.stream_name	= "Capture",
		.channels_min	= 1,
		.channels_max	= 2,
		.rates 		= SUN4I_CODEC_RATES,
		.formats 	= SUN4I_CODEC_FORMATS,
		.sig_bits	= 24,
	 },
};

static struct snd_soc_dai_link *sun4i_codec_create_link(struct device *dev,
							int *num_links)
{
	struct snd_soc_dai_link *link = devm_kzalloc(dev, sizeof(*link),
						     GFP_KERNEL);
	if (!link)
		return NULL;

	link->name		= "cdc";
	link->stream_name	= "CDC PCM";
	link->codec_dai_name	= "Codec";
	link->cpu_dai_name	= dev_name(dev);
	link->codec_name	= dev_name(dev);
	link->platform_name	= dev_name(dev);
	link->dai_fmt		= SND_SOC_DAIFMT_I2S;

	*num_links = 1;

	return link;
};

static int sun4i_codec_spk_event(struct snd_soc_dapm_widget *w,
				 struct snd_kcontrol *k, int event)
{
	struct sun4i_codec *scodec = snd_soc_card_get_drvdata(w->dapm->card);

	gpiod_set_value_cansleep(scodec->gpio_pa,
				 !!SND_SOC_DAPM_EVENT_ON(event));

	return 0;
}

static const struct snd_soc_dapm_widget sun4i_codec_card_dapm_widgets[] = {
	SND_SOC_DAPM_SPK("Speaker", sun4i_codec_spk_event),
};

static const struct snd_soc_dapm_route sun4i_codec_card_dapm_routes[] = {
	{ "Speaker", NULL, "HP Right" },
	{ "Speaker", NULL, "HP Left" },
};

static struct snd_soc_card *sun4i_codec_create_card(struct device *dev)
{
	struct snd_soc_card *card;

	card = devm_kzalloc(dev, sizeof(*card), GFP_KERNEL);
	if (!card)
		return ERR_PTR(-ENOMEM);

	card->dai_link = sun4i_codec_create_link(dev, &card->num_links);
	if (!card->dai_link)
		return ERR_PTR(-ENOMEM);

	card->dev		= dev;
	card->name		= "sun4i-codec";
	card->dapm_widgets	= sun4i_codec_card_dapm_widgets;
	card->num_dapm_widgets	= ARRAY_SIZE(sun4i_codec_card_dapm_widgets);
	card->dapm_routes	= sun4i_codec_card_dapm_routes;
	card->num_dapm_routes	= ARRAY_SIZE(sun4i_codec_card_dapm_routes);

	return card;
};

static const struct snd_soc_dapm_widget sun6i_codec_card_dapm_widgets[] = {
	SND_SOC_DAPM_HP("Headphone", NULL),
	SND_SOC_DAPM_LINE("Line In", NULL),
	SND_SOC_DAPM_LINE("Line Out", NULL),
	SND_SOC_DAPM_MIC("Headset Mic", NULL),
	SND_SOC_DAPM_MIC("Mic", NULL),
	SND_SOC_DAPM_SPK("Speaker", sun4i_codec_spk_event),
};

static struct snd_soc_card *sun6i_codec_create_card(struct device *dev)
{
	struct snd_soc_card *card;
	int ret;

	card = devm_kzalloc(dev, sizeof(*card), GFP_KERNEL);
	if (!card)
		return ERR_PTR(-ENOMEM);

	card->dai_link = sun4i_codec_create_link(dev, &card->num_links);
	if (!card->dai_link)
		return ERR_PTR(-ENOMEM);

	card->dev		= dev;
	card->name		= "A31 Audio Codec";
	card->dapm_widgets	= sun6i_codec_card_dapm_widgets;
	card->num_dapm_widgets	= ARRAY_SIZE(sun6i_codec_card_dapm_widgets);
	card->fully_routed	= true;

	ret = snd_soc_of_parse_audio_routing(card, "allwinner,audio-routing");
	if (ret)
		dev_warn(dev, "failed to parse audio-routing: %d\n", ret);

	return card;
};

/* Connect digital side enables to analog side widgets */
static const struct snd_soc_dapm_route sun8i_codec_card_routes[] = {
	/* ADC Routes */
	{ "Left ADC", NULL, "ADC Enable" },
	{ "Right ADC", NULL, "ADC Enable" },
	{ "Codec Capture", NULL, "Left ADC" },
	{ "Codec Capture", NULL, "Right ADC" },

	/* DAC Routes */
	{ "Left DAC", NULL, "DAC Enable" },
	{ "Right DAC", NULL, "DAC Enable" },
	{ "Left DAC", NULL, "Codec Playback" },
	{ "Right DAC", NULL, "Codec Playback" },
};

static struct snd_soc_aux_dev aux_dev = {
	.name = "Codec Analog Controls",
};

static struct snd_soc_card *sun8i_a23_codec_create_card(struct device *dev)
{
	struct snd_soc_card *card;
	int ret;

	card = devm_kzalloc(dev, sizeof(*card), GFP_KERNEL);
	if (!card)
		return ERR_PTR(-ENOMEM);

	aux_dev.codec_of_node = of_parse_phandle(dev->of_node,
						 "allwinner,codec-analog-controls",
						 0);
	if (!aux_dev.codec_of_node) {
		dev_err(dev, "Can't find analog controls for codec.\n");
		return ERR_PTR(-EINVAL);
	};

	card->dai_link = sun4i_codec_create_link(dev, &card->num_links);
	if (!card->dai_link)
		return ERR_PTR(-ENOMEM);

	card->dev		= dev;
	card->name		= "A23 Audio Codec";
	card->dapm_widgets	= sun6i_codec_card_dapm_widgets;
	card->num_dapm_widgets	= ARRAY_SIZE(sun6i_codec_card_dapm_widgets);
	card->dapm_routes	= sun8i_codec_card_routes;
	card->num_dapm_routes	= ARRAY_SIZE(sun8i_codec_card_routes);
	card->aux_dev		= &aux_dev;
	card->num_aux_devs	= 1;
	card->fully_routed	= true;

	ret = snd_soc_of_parse_audio_routing(card, "allwinner,audio-routing");
	if (ret)
		dev_warn(dev, "failed to parse audio-routing: %d\n", ret);

	return card;
};

static struct snd_soc_card *sun8i_h3_codec_create_card(struct device *dev)
{
	struct snd_soc_card *card;
	int ret;

	card = devm_kzalloc(dev, sizeof(*card), GFP_KERNEL);
	if (!card)
		return ERR_PTR(-ENOMEM);

	aux_dev.codec_of_node = of_parse_phandle(dev->of_node,
						 "allwinner,codec-analog-controls",
						 0);
	if (!aux_dev.codec_of_node) {
		dev_err(dev, "Can't find analog controls for codec.\n");
		return ERR_PTR(-EINVAL);
	};

	card->dai_link = sun4i_codec_create_link(dev, &card->num_links);
	if (!card->dai_link)
		return ERR_PTR(-ENOMEM);

	card->dev		= dev;
	card->name		= "H3 Audio Codec";
	card->dapm_widgets	= sun6i_codec_card_dapm_widgets;
	card->num_dapm_widgets	= ARRAY_SIZE(sun6i_codec_card_dapm_widgets);
	card->dapm_routes	= sun8i_codec_card_routes;
	card->num_dapm_routes	= ARRAY_SIZE(sun8i_codec_card_routes);
	card->aux_dev		= &aux_dev;
	card->num_aux_devs	= 1;
	card->fully_routed	= true;

	ret = snd_soc_of_parse_audio_routing(card, "allwinner,audio-routing");
	if (ret)
		dev_warn(dev, "failed to parse audio-routing: %d\n", ret);

	return card;
};

static struct snd_soc_card *sun8i_v3s_codec_create_card(struct device *dev)
{
	struct snd_soc_card *card;
	int ret;

	card = devm_kzalloc(dev, sizeof(*card), GFP_KERNEL);
	if (!card)
		return ERR_PTR(-ENOMEM);

	aux_dev.codec_of_node = of_parse_phandle(dev->of_node,
						 "allwinner,codec-analog-controls",
						 0);
	if (!aux_dev.codec_of_node) {
		dev_err(dev, "Can't find analog controls for codec.\n");
		return ERR_PTR(-EINVAL);
	};

	card->dai_link = sun4i_codec_create_link(dev, &card->num_links);
	if (!card->dai_link)
		return ERR_PTR(-ENOMEM);

	card->dev		= dev;
	card->name		= "V3s Audio Codec";
	card->dapm_widgets	= sun6i_codec_card_dapm_widgets;
	card->num_dapm_widgets	= ARRAY_SIZE(sun6i_codec_card_dapm_widgets);
	card->dapm_routes	= sun8i_codec_card_routes;
	card->num_dapm_routes	= ARRAY_SIZE(sun8i_codec_card_routes);
	card->aux_dev		= &aux_dev;
	card->num_aux_devs	= 1;
	card->fully_routed	= true;

	ret = snd_soc_of_parse_audio_routing(card, "allwinner,audio-routing");
	if (ret)
		dev_warn(dev, "failed to parse audio-routing: %d\n", ret);

	return card;
};

static const struct snd_soc_dapm_widget suniv_codec_card_dapm_widgets[] = {
	SND_SOC_DAPM_HP("Headphone", NULL),
	SND_SOC_DAPM_LINE("Line In", NULL),
	SND_SOC_DAPM_LINE("Right FM In", NULL),
	SND_SOC_DAPM_LINE("Left FM In", NULL),
	SND_SOC_DAPM_MIC("Mic", NULL),
	SND_SOC_DAPM_SPK("Speaker", sun4i_codec_spk_event),
};

/* Connect digital side enables to analog side widgets */
static const struct snd_soc_dapm_route suniv_codec_card_routes[] = {
	/* ADC Routes */
	{ "ADC", NULL, "ADC Enable" },
	{ "Codec Capture", NULL, "ADC" },

	/* DAC Routes */
	{ "Left DAC", NULL, "DAC Enable" },
	{ "Right DAC", NULL, "DAC Enable" },
	{ "Left DAC", NULL, "Codec Playback" },
	{ "Right DAC", NULL, "Codec Playback" },
};


static struct snd_soc_card *suniv_codec_create_card(struct device *dev)
{
	struct snd_soc_card *card;
	int ret;

	card = devm_kzalloc(dev, sizeof(*card), GFP_KERNEL);
	if (!card)
		return ERR_PTR(-ENOMEM);

	card->dai_link = sun4i_codec_create_link(dev, &card->num_links);
	if (!card->dai_link)
		return ERR_PTR(-ENOMEM);

	card->dev = dev;
	card->name = "F1C100s Audio Codec";
	card->dapm_widgets = suniv_codec_card_dapm_widgets;
	card->num_dapm_widgets = ARRAY_SIZE(suniv_codec_card_dapm_widgets);
	card->dapm_routes = suniv_codec_card_routes;
	card->num_dapm_routes = ARRAY_SIZE(suniv_codec_card_routes);
	card->fully_routed = true;

	ret = snd_soc_of_parse_audio_routing(card, "allwinner,audio-routing");
	if (ret)
		dev_warn(dev, "failed to parse audio-routing: %d\n", ret);

	return card;
};

static const struct regmap_config sun4i_codec_regmap_config = {
	.reg_bits	= 32,
	.reg_stride	= 4,
	.val_bits	= 32,
	.max_register	= SUN4I_CODEC_ADC_RXCNT,
};

static const struct regmap_config sun6i_codec_regmap_config = {
	.reg_bits	= 32,
	.reg_stride	= 4,
	.val_bits	= 32,
	.max_register	= SUN6I_CODEC_HMIC_DATA,
};

static const struct regmap_config sun7i_codec_regmap_config = {
	.reg_bits	= 32,
	.reg_stride	= 4,
	.val_bits	= 32,
	.max_register	= SUN7I_CODEC_AC_MIC_PHONE_CAL,
};

static const struct regmap_config sun8i_a23_codec_regmap_config = {
	.reg_bits	= 32,
	.reg_stride	= 4,
	.val_bits	= 32,
	.max_register	= SUN8I_A23_CODEC_ADC_RXCNT,
};

static const struct regmap_config sun8i_h3_codec_regmap_config = {
	.reg_bits	= 32,
	.reg_stride	= 4,
	.val_bits	= 32,
	.max_register	= SUN8I_H3_CODEC_ADC_DBG,
};

static const struct regmap_config sun8i_v3s_codec_regmap_config = {
	.reg_bits	= 32,
	.reg_stride	= 4,
	.val_bits	= 32,
	.max_register	= SUN8I_H3_CODEC_ADC_DBG,
};

static const struct regmap_config suniv_codec_regmap_config = {
	.reg_bits = 32,
	.reg_stride = 4,
	.val_bits = 32,
	.max_register = SUNIV_CODEC_ADC_DBG,
};

struct sun4i_codec_quirks {
	const struct regmap_config *regmap_config;
	const struct snd_soc_component_driver *codec;
	struct snd_soc_card * (*create_card)(struct device *dev);
	struct reg_field reg_adc_fifoc;	/* used for regmap_field */
	unsigned int reg_dac_txdata;	/* TX FIFO offset for DMA config */
	unsigned int reg_adc_rxdata;	/* RX FIFO offset for DMA config */
	bool has_reset;
	u32 dma_max_burst;
};

static const struct sun4i_codec_quirks sun4i_codec_quirks = {
	.regmap_config	= &sun4i_codec_regmap_config,
	.codec		= &sun4i_codec_codec,
	.create_card	= sun4i_codec_create_card,
	.reg_adc_fifoc	= REG_FIELD(SUN4I_CODEC_ADC_FIFOC, 0, 31),
	.reg_dac_txdata	= SUN4I_CODEC_DAC_TXDATA,
	.reg_adc_rxdata	= SUN4I_CODEC_ADC_RXDATA,
	.dma_max_burst = SUN4I_DMA_MAX_BURST,
};

static const struct sun4i_codec_quirks sun6i_a31_codec_quirks = {
	.regmap_config	= &sun6i_codec_regmap_config,
	.codec		= &sun6i_codec_codec,
	.create_card	= sun6i_codec_create_card,
	.reg_adc_fifoc	= REG_FIELD(SUN6I_CODEC_ADC_FIFOC, 0, 31),
	.reg_dac_txdata	= SUN4I_CODEC_DAC_TXDATA,
	.reg_adc_rxdata	= SUN6I_CODEC_ADC_RXDATA,
	.has_reset	= true,
	.dma_max_burst = SUN4I_DMA_MAX_BURST,
};

static const struct sun4i_codec_quirks sun7i_codec_quirks = {
	.regmap_config	= &sun7i_codec_regmap_config,
	.codec		= &sun4i_codec_codec,
	.create_card	= sun4i_codec_create_card,
	.reg_adc_fifoc	= REG_FIELD(SUN4I_CODEC_ADC_FIFOC, 0, 31),
	.reg_dac_txdata	= SUN4I_CODEC_DAC_TXDATA,
	.reg_adc_rxdata	= SUN4I_CODEC_ADC_RXDATA,
	.dma_max_burst = SUN4I_DMA_MAX_BURST,
};

static const struct sun4i_codec_quirks sun8i_a23_codec_quirks = {
	.regmap_config	= &sun8i_a23_codec_regmap_config,
	.codec		= &sun8i_a23_codec_codec,
	.create_card	= sun8i_a23_codec_create_card,
	.reg_adc_fifoc	= REG_FIELD(SUN6I_CODEC_ADC_FIFOC, 0, 31),
	.reg_dac_txdata	= SUN4I_CODEC_DAC_TXDATA,
	.reg_adc_rxdata	= SUN6I_CODEC_ADC_RXDATA,
	.has_reset	= true,
	.dma_max_burst = SUN4I_DMA_MAX_BURST,
};

static const struct sun4i_codec_quirks sun8i_h3_codec_quirks = {
	.regmap_config	= &sun8i_h3_codec_regmap_config,
	/*
	 * TODO Share the codec structure with A23 for now.
	 * This should be split out when adding digital audio
	 * processing support for the H3.
	 */
	.codec		= &sun8i_a23_codec_codec,
	.create_card	= sun8i_h3_codec_create_card,
	.reg_adc_fifoc	= REG_FIELD(SUN6I_CODEC_ADC_FIFOC, 0, 31),
	.reg_dac_txdata	= SUN8I_H3_CODEC_DAC_TXDATA,
	.reg_adc_rxdata	= SUN6I_CODEC_ADC_RXDATA,
	.has_reset	= true,
	.dma_max_burst = SUN4I_DMA_MAX_BURST,
};

static const struct sun4i_codec_quirks sun8i_v3s_codec_quirks = {
	.regmap_config	= &sun8i_v3s_codec_regmap_config,
	/*
	 * TODO The codec structure should be split out, like
	 * H3, when adding digital audio processing support.
	 */
	.codec		= &sun8i_a23_codec_codec,
	.create_card	= sun8i_v3s_codec_create_card,
	.reg_adc_fifoc	= REG_FIELD(SUN6I_CODEC_ADC_FIFOC, 0, 31),
	.reg_dac_txdata	= SUN8I_H3_CODEC_DAC_TXDATA,
	.reg_adc_rxdata	= SUN6I_CODEC_ADC_RXDATA,
	.has_reset	= true,
	.dma_max_burst = SUN4I_DMA_MAX_BURST,
};

static const struct sun4i_codec_quirks suniv_f1c100s_codec_quirks = {
	.regmap_config = &suniv_codec_regmap_config,
	.codec = &suniv_codec_codec,
	.create_card = suniv_codec_create_card,
	.reg_adc_fifoc = REG_FIELD(SUNIV_CODEC_ADC_FIFOC, 0, 31),
	.reg_dac_txdata = SUN4I_CODEC_DAC_TXDATA,
	.reg_adc_rxdata = SUNIV_CODEC_ADC_RXDATA,
	.has_reset = true,
	.dma_max_burst = SUNIV_DMA_MAX_BURST,
};

static const struct of_device_id sun4i_codec_of_match[] = {
	{
		.compatible = "allwinner,sun4i-a10-codec",
		.data = &sun4i_codec_quirks,
	},
	{
		.compatible = "allwinner,sun6i-a31-codec",
		.data = &sun6i_a31_codec_quirks,
	},
	{
		.compatible = "allwinner,sun7i-a20-codec",
		.data = &sun7i_codec_quirks,
	},
	{
		.compatible = "allwinner,sun8i-a23-codec",
		.data = &sun8i_a23_codec_quirks,
	},
	{
		.compatible = "allwinner,sun8i-h3-codec",
		.data = &sun8i_h3_codec_quirks,
	},
	{
		.compatible = "allwinner,sun8i-v3s-codec",
		.data = &sun8i_v3s_codec_quirks,
	},
	{
		.compatible = "allwinner,suniv-codec",
		.data = &suniv_f1c100s_codec_quirks,
	},
	{}
};
MODULE_DEVICE_TABLE(of, sun4i_codec_of_match);

static int sun4i_codec_probe(struct platform_device *pdev)
{
	struct snd_soc_card *card;
	struct sun4i_codec *scodec;
	const struct sun4i_codec_quirks *quirks;
	struct resource *res;
	void __iomem *base;
	int ret;

	scodec = devm_kzalloc(&pdev->dev, sizeof(*scodec), GFP_KERNEL);
	if (!scodec)
		return -ENOMEM;

	scodec->dev = &pdev->dev;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(base)) {
		dev_err(&pdev->dev, "Failed to map the registers\n");
		return PTR_ERR(base);
	}

	quirks = of_device_get_match_data(&pdev->dev);
	if (quirks == NULL) {
		dev_err(&pdev->dev, "Failed to determine the quirks to use\n");
		return -ENODEV;
	}

	scodec->regmap = devm_regmap_init_mmio(&pdev->dev, base,
					       quirks->regmap_config);
	if (IS_ERR(scodec->regmap)) {
		dev_err(&pdev->dev, "Failed to create our regmap\n");
		return PTR_ERR(scodec->regmap);
	}

	/* Get the clocks from the DT */
	scodec->clk_apb = devm_clk_get(&pdev->dev, "apb");
	if (IS_ERR(scodec->clk_apb)) {
		dev_err(&pdev->dev, "Failed to get the APB clock\n");
		return PTR_ERR(scodec->clk_apb);
	}

	scodec->clk_module = devm_clk_get(&pdev->dev, "codec");
	if (IS_ERR(scodec->clk_module)) {
		dev_err(&pdev->dev, "Failed to get the module clock\n");
		return PTR_ERR(scodec->clk_module);
	}

	if (quirks->has_reset) {
		scodec->rst = devm_reset_control_get_exclusive(&pdev->dev,
							       NULL);
		if (IS_ERR(scodec->rst)) {
			dev_err(&pdev->dev, "Failed to get reset control\n");
			return PTR_ERR(scodec->rst);
		}
	}

	scodec->gpio_pa = devm_gpiod_get_optional(&pdev->dev, "allwinner,pa",
						  GPIOD_OUT_LOW);
	if (IS_ERR(scodec->gpio_pa)) {
		ret = PTR_ERR(scodec->gpio_pa);
		if (ret != -EPROBE_DEFER)
			dev_err(&pdev->dev, "Failed to get pa gpio: %d\n", ret);
		return ret;
	}

	/* reg_field setup */
	scodec->reg_adc_fifoc = devm_regmap_field_alloc(&pdev->dev,
							scodec->regmap,
							quirks->reg_adc_fifoc);
	if (IS_ERR(scodec->reg_adc_fifoc)) {
		ret = PTR_ERR(scodec->reg_adc_fifoc);
		dev_err(&pdev->dev, "Failed to create regmap fields: %d\n",
			ret);
		return ret;
	}

	/* Enable the bus clock */
	if (clk_prepare_enable(scodec->clk_apb)) {
		dev_err(&pdev->dev, "Failed to enable the APB clock\n");
		return -EINVAL;
	}

	/* Deassert the reset control */
	if (scodec->rst) {
		ret = reset_control_deassert(scodec->rst);
		if (ret) {
			dev_err(&pdev->dev,
				"Failed to deassert the reset control\n");
			goto err_clk_disable;
		}
	}

	/* DMA configuration for TX FIFO */
	scodec->playback_dma_data.addr = res->start + quirks->reg_dac_txdata;
	scodec->playback_dma_data.maxburst = quirks->dma_max_burst;
	scodec->playback_dma_data.addr_width = DMA_SLAVE_BUSWIDTH_2_BYTES;

	/* DMA configuration for RX FIFO */
	scodec->capture_dma_data.addr = res->start + quirks->reg_adc_rxdata;
	scodec->capture_dma_data.maxburst = quirks->dma_max_burst;
	scodec->capture_dma_data.addr_width = DMA_SLAVE_BUSWIDTH_2_BYTES;

	ret = devm_snd_soc_register_component(&pdev->dev, quirks->codec,
				     &sun4i_codec_dai, 1);
	if (ret) {
		dev_err(&pdev->dev, "Failed to register our codec\n");
		goto err_assert_reset;
	}

	ret = devm_snd_soc_register_component(&pdev->dev,
					      &sun4i_codec_component,
					      &dummy_cpu_dai, 1);
	if (ret) {
		dev_err(&pdev->dev, "Failed to register our DAI\n");
		goto err_assert_reset;
	}

	ret = devm_snd_dmaengine_pcm_register(&pdev->dev, NULL, 0);
	if (ret) {
		dev_err(&pdev->dev, "Failed to register against DMAEngine\n");
		goto err_assert_reset;
	}

	card = quirks->create_card(&pdev->dev);
	if (IS_ERR(card)) {
		ret = PTR_ERR(card);
		dev_err(&pdev->dev, "Failed to create our card\n");
		goto err_assert_reset;
	}

	snd_soc_card_set_drvdata(card, scodec);

	ret = snd_soc_register_card(card);
	if (ret) {
		dev_err(&pdev->dev, "Failed to register our card\n");
		goto err_assert_reset;
	}

	return 0;

err_assert_reset:
	if (scodec->rst)
		reset_control_assert(scodec->rst);
err_clk_disable:
	clk_disable_unprepare(scodec->clk_apb);
	return ret;
}

static int sun4i_codec_remove(struct platform_device *pdev)
{
	struct snd_soc_card *card = platform_get_drvdata(pdev);
	struct sun4i_codec *scodec = snd_soc_card_get_drvdata(card);

	snd_soc_unregister_card(card);
	if (scodec->rst)
		reset_control_assert(scodec->rst);
	clk_disable_unprepare(scodec->clk_apb);

	return 0;
}

static struct platform_driver sun4i_codec_driver = {
	.driver = {
		.name = "sun4i-codec",
		.of_match_table = sun4i_codec_of_match,
	},
	.probe = sun4i_codec_probe,
	.remove = sun4i_codec_remove,
};
module_platform_driver(sun4i_codec_driver);

MODULE_DESCRIPTION("Allwinner A10 codec driver");
MODULE_AUTHOR("Emilio López <emilio@elopez.com.ar>");
MODULE_AUTHOR("Jon Smirl <jonsmirl@gmail.com>");
MODULE_AUTHOR("Maxime Ripard <maxime.ripard@free-electrons.com>");
MODULE_AUTHOR("Chen-Yu Tsai <wens@csie.org>");
MODULE_AUTHOR("Mesih Kilinc <***@gmail.com>");
MODULE_LICENSE("GPL");
