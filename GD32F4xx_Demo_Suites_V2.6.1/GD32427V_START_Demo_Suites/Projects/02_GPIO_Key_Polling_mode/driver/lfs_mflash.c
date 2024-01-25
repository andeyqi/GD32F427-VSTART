#include "lfs_mflash.h"
#include "fmc_operation.h"

#define LITTLEFS_START_ADDR 0x08108000


/*******************************************************************************
 * Variables
 ******************************************************************************/

struct lfs_mflash_ctx LittleFS_ctx = {LITTLEFS_START_ADDR};


int lfs_mflash_erase(const struct lfs_config *lfsc, lfs_block_t block)
{
    fmc_state_enum fmc_state; 
    uint32_t page_addr;
    struct lfs_mflash_ctx *ctx;
    
    ctx = (struct lfs_mflash_ctx *)lfsc->context;
    page_addr = ctx->start_addr + block * lfsc->block_size;
    /* unlock the flash program erase controller */
    fmc_unlock();
    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);
    fmc_state = fmc_page_erase(page_addr);
    /* lock the flash program erase controller */
    fmc_lock();
    if (fmc_state != FMC_READY)
        return LFS_ERR_IO;

    return LFS_ERR_OK;

}


int lfs_mflash_read(const struct lfs_config *lfsc, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    struct lfs_mflash_ctx *ctx;
    uint32_t flash_addr;

    assert(lfsc);
    ctx = (struct lfs_mflash_ctx *)lfsc->context;
    assert(ctx);

    flash_addr = ctx->start_addr + block * lfsc->block_size + off;
    
    for(lfs_size_t i=0; i < size; i++)
    {
        ((int8_t *)buffer)[i] = *((__IO int8_t*)flash_addr);
        flash_addr++;
    }

    return LFS_ERR_OK;
}


int lfs_mflash_prog(
    const struct lfs_config *lfsc, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{ 
    struct lfs_mflash_ctx *ctx;
    uint32_t flash_addr;

    assert(lfsc);
    ctx = (struct lfs_mflash_ctx *)lfsc->context;
    assert(ctx);

    flash_addr = ctx->start_addr + block * lfsc->block_size + off;
   
    /* unlock the flash program erase controller */
    fmc_unlock();
    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);

    /* write data_8 to the corresponding address */
    for(uint32_t i=0; i < size; i++){
        if(FMC_READY == fmc_byte_program(flash_addr, ((uint8_t *)buffer)[i])){
            flash_addr++;
        }else{
            while(1);
        }
    }
    /* lock the flash program erase controller */
    fmc_lock();
    
    return LFS_ERR_OK;
}


int lfs_mflash_sync(const struct lfs_config *lfsc)
{
    return LFS_ERR_OK;
}



const struct lfs_config LittleFS_config = {
  .context = (void*)&LittleFS_ctx,
  .read = lfs_mflash_read,
  .prog = lfs_mflash_prog,
  .erase = lfs_mflash_erase,
  .sync = lfs_mflash_sync,
  .read_size = 1,
  .prog_size = 1,
  .block_size = 4096,
  .block_count = 512,
  .block_cycles = 100,
  .cache_size = 128,
  .lookahead_size = 128
};




int lfs_get_default_config(struct lfs_config *lfsc)
{
    *lfsc = LittleFS_config; /* copy pre-initialized lfs config structure */
    return 0;
}
