/*
 * Copyright (C) 2012 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/uaccess.h>
#include <linux/sprd_mm.h>
#include <video/sprd_isp.h>
#include "isp_reg.h"

static int32_t isp_k_fcs_block(struct isp_io_param *param)
{
	int32_t ret = 0;
	struct isp_dev_fcs_info fcs_info;

	memset(&fcs_info, 0x00, sizeof(fcs_info));

	ret = copy_from_user((void *)&fcs_info, param->property_param, sizeof(fcs_info));
	if (0 != ret) {
		printk("isp_k_fcs_bypass: copy error, ret=0x%x\n", (uint32_t)ret);
		return -1;
	}

	REG_MWR(ISP_FCS_PARAM, BIT_16, (fcs_info.mode << 16));

	if (fcs_info.bypass) {
		REG_OWR(ISP_FCS_PARAM, BIT_0);
	} else {
		REG_MWR(ISP_FCS_PARAM, BIT_0, 0);
	}

	return ret;
}

static int32_t isp_k_fcs_bypass(struct isp_io_param *param)
{
	int32_t ret = 0;
	uint32_t bypass = 0;

	ret = copy_from_user((void *)&bypass, param->property_param, sizeof(bypass));
	if (0 != ret) {
		printk("isp_k_fcs_bypass: copy error, ret=0x%x\n", (uint32_t)ret);
		return -1;
	}

	if (bypass) {
		REG_OWR(ISP_FCS_PARAM, BIT_0);
	} else {
		REG_MWR(ISP_FCS_PARAM, BIT_0, 0);
	}

	return ret;
}

static int32_t isp_k_fcs_mode(struct isp_io_param *param)
{
	int32_t ret = 0;
	uint32_t mode = 0;

	ret = copy_from_user((void *)&mode, param->property_param, sizeof(mode));
	if (0 != ret) {
		printk("isp_k_fcs_mode: copy error, ret=0x%x\n", (uint32_t)ret);
		return -1;
	}

	REG_MWR(ISP_FCS_PARAM, BIT_16, (mode << 16));

	return ret;
}

int32_t isp_k_cfg_fcs(struct isp_io_param *param)
{
	int32_t ret = 0;

	if (!param) {
		printk("isp_k_cfg_fcs: param is null error.\n");
		return -1;
	}

	if (NULL == param->property_param) {
		printk("isp_k_cfg_fcs: property_param is null error.\n");
		return -1;
	}

	switch (param->property) {
	case ISP_PRO_FCS_BLOCK:
		ret = isp_k_fcs_block(param);
		break;
	case ISP_PRO_FCS_BYPASS:
		ret = isp_k_fcs_bypass(param);
		break;
	case ISP_PRO_FCS_MODE:
		ret = isp_k_fcs_mode(param);
		break;
	default:
		printk("isp_k_cfg_fcs: fail cmd id:%d, not supported.\n", param->property);
		break;
	}

	return ret;
}
