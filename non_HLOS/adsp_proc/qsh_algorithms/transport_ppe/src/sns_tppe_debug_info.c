/*=============================================================================
  @file sns_tppe_debug_info.c

  Contains function definitions required for duplicate detection and filtering

  Copyright (c) 2022-2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
  ===========================================================================*/
#include "sns_tppe_debug_dump_util.h"
#include "sns_tppe_debug_info.h"
#include "sns_types.h"

#ifndef SNS_TPPE_DEBUG_PRINT

void print_all_triggers(sns_sensor_instance *const this)
{
  UNUSED_VAR(this);
}

#else

static void print_bytes(sns_sensor_instance *const this, uint8_t *buf, size_t len, char prefix)
{
  for(int i = 0; i < len; i++)
  {
    SNS_TPPE_INST_PRINTF(MED, this, "%c[%d]=%d", prefix, i, buf[i]);
  }
}

void print_all_triggers(sns_sensor_instance *const this)
{
  sns_tppe_inst_state *inst_state = (sns_tppe_inst_state *)this->state->state;
  sns_all_filters *all_filters = &inst_state->all_filters;
  sns_prop_filter *prop_filters = all_filters->prop_filters;
  sns_data_filter *service_data_filters = all_filters->service_data_filters;
  sns_data_filter *manuf_data_filters = all_filters->manuf_data_filters;
  sns_single_trigger *triggers = all_filters->wakeup_triggers;
  uint32_t *big1 = NULL;
  uint32_t *big2 = NULL;
  size_t total_pf_count = all_filters->prop_filter_count + all_filters->dup_pf_count;
  size_t total_sdf_count = all_filters->service_df_count + all_filters->dup_sdf_count;
  size_t total_mdf_count = all_filters->manuf_df_count + all_filters->dup_mdf_count;
  int i = 0;

  for(i = 0; i < all_filters->trigger_count; i++)
  {
    SNS_TPPE_INST_PRINTF(MED, this, "---------trigger[%d]-------------", i);
    SNS_TPPE_INST_PRINTF(MED, this, "triggerid %d dup_detect_period %u", triggers[i].trigger_id,
                         triggers[i].dup_detect_period);
    SNS_TPPE_INST_PRINTF(MED, this, "pf_count %d sdf_count %d mdf count %d", triggers[i].trigger_id,
                         triggers[i].prop_filter_count, triggers[i].service_df_count,
                         triggers[i].manuf_df_count);
    SNS_TPPE_INST_PRINTF(MED, this, "dup_pf_count %d dup_sdf_count %d dup_mdf count %d",
                         triggers[i].dup_pf_count, triggers[i].dup_sdf_count,
                         triggers[i].dup_mdf_count);
  }

  for(i = 0; i < total_pf_count; i++)
  {
    SNS_TPPE_INST_PRINTF(MED, this, "---------property filter[%d]-------------", i);
    SNS_TPPE_INST_PRINTF(MED, this, "threshold,prop_type = %d,%d",
                         prop_filters[i].prop_filter.threshold,
                         prop_filters[i].prop_filter.prop.which_prop);

    print_bytes(this, (uint8_t *)&prop_filters[i].prop_filter.prop.prop,
                sizeof(prop_filters[i].prop_filter.prop.prop), 'P');
  }

  for(i = 0; i < total_sdf_count; i++)
  {
    big1 = (uint32_t *)&service_data_filters[i].id.id_low;
    big2 = (uint32_t *)&service_data_filters[i].id.id_high;
    SNS_TPPE_INST_PRINTF(MED, this,
                         "---------service df[%d] id_low 0x%X 0x%X id_high 0x%X 0x%X-----------", i,
                         big1[0], big1[1], big2[0], big2[1]);
    print_bytes(this, service_data_filters[i].value, service_data_filters[i].bytes_size, 'V');
    print_bytes(this, service_data_filters[i].mask, service_data_filters[i].bytes_size, 'M');
  }
  for(i = 0; i < total_mdf_count; i++)
  {
    big1 = (uint32_t *)&manuf_data_filters[i].id.id_low;
    SNS_TPPE_INST_PRINTF(MED, this, "---------service df[%d] id_low 0x%X 0x%X -----------", i,
                         big1[0], big1[1]);
    print_bytes(this, manuf_data_filters[i].value, manuf_data_filters[i].bytes_size, 'V');
    print_bytes(this, manuf_data_filters[i].mask, manuf_data_filters[i].bytes_size, 'M');
  }
}

void handle_debug_dump_request(sns_sensor_instance *const this)
{
  SNS_TPPE_INST_PRINTF(MED, this, "handle debug dump request called");
  sns_tppe_inst_state *inst_state = (sns_tppe_inst_state *)this->state->state;
  sns_all_filters *all_filters = &inst_state->all_filters;
  sns_prop_filter *prop_filters = all_filters->prop_filters;
  sns_data_filter *service_data_filters = all_filters->service_data_filters;
  sns_data_filter *manuf_data_filters = all_filters->manuf_data_filters;
  sns_single_trigger *triggers = all_filters->wakeup_triggers;
  size_t vm_index = 0;
  uint8_t *value_mask_array = all_filters->value_mask_array;
  uint32_t *big_number;
  sns_data_filter *df;
  size_t total_pf_count = all_filters->prop_filter_count + all_filters->dup_pf_count;
  size_t total_sdf_count = all_filters->service_df_count + all_filters->dup_sdf_count;
  size_t total_mdf_count = all_filters->manuf_df_count + all_filters->dup_mdf_count;
  int i = 0;

  sns_debug_dump_util dd_util;
  sns_ddu_init(&dd_util, this);
  sns_ddu_dump_str(&dd_util, "tid,pf,sdf,mdf,dpf,dmdf,dsdf\n");
  for(i = 0; i < all_filters->trigger_count; i++)
  {
    sns_ddu_dump_str(&dd_util, "%d,%d,%d,%d,%d,%d,%d\n", triggers[i].trigger_id,
                     triggers[i].prop_filter_count, triggers[i].service_df_count,
                     triggers[i].manuf_df_count, triggers[i].dup_pf_count,
                     triggers[i].dup_sdf_count, triggers[i].dup_mdf_count);
  }

  sns_ddu_dump_str(&dd_util, "property,threshold,value_low,value_high\n");
  for(i = 0; i < total_pf_count; i++)
  {
    big_number = (uint32_t *)&prop_filters[i].prop_filter.prop.prop.timestamp;
    sns_ddu_dump_str(&dd_util, "%d,%d,0x%X,0x%X\n", prop_filters[i].prop_filter.threshold,
                     prop_filters[i].prop_filter.prop.which_prop, big_number[0], big_number[1]);
  }

  sns_ddu_dump_str(&dd_util, "Data Filters:\n");
  sns_ddu_dump_str(&dd_util, "type,idhigh,idlow,value,mask");
  for(i = 0; i < total_sdf_count; i++)
  {
    df = &service_data_filters[i];
    sns_ddu_dump_str(&dd_util, "\nservice,0x%X,0x%X,", df->id.id_high, df->id.id_low);
    // value
    for(int j = 0; j < df->bytes_size; j++)
    {
      sns_ddu_dump_str(&dd_util, "0x%02X ", value_mask_array[vm_index]);
      vm_index++;
    }
    // mask
    sns_ddu_dump_str(&dd_util, ",");
    for(int j = 0; j < df->bytes_size; j++)
    {
      sns_ddu_dump_str(&dd_util, "0x%02X ", value_mask_array[vm_index]);
      vm_index++;
    }
  }

  for(i = 0; i < total_mdf_count; i++)
  {
    df = &manuf_data_filters[i];
    sns_ddu_dump_str(&dd_util, "\nmanuf,0x%X,0x%X,", df->id.id_high, df->id.id_low);
    // value
    for(int j = 0; j < df->bytes_size; j++)
    {
      sns_ddu_dump_str(&dd_util, "0x%02X ", value_mask_array[vm_index]);
      vm_index++;
    }
    // mask
    sns_ddu_dump_str(&dd_util, ",");
    for(int j = 0; j < df->bytes_size; j++)
    {
      sns_ddu_dump_str(&dd_util, "0x%02X ", value_mask_array[vm_index]);
      vm_index++;
    }
  }

  sns_ddu_deinit(&dd_util);
}

#endif // SNS_TPPE_DEBUG_PRINT
