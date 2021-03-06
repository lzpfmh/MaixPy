#include "w25qxx.h"
#include "spiffs-port.h"
#include "spiffs_config.h"
#include <stdio.h>
#include "sleep.h"

#define foce_format_fs 0

spiffs fs;

static u8_t spiffs_work_buf[SPIFFS_CFG_LOG_PAGE_SZ(fs)*2];
static u8_t spiffs_fds[32*4];
static u8_t spiffs_cache_buf[(SPIFFS_CFG_LOG_PAGE_SZ(fs)+32)*4];

s32_t k210_sf_read(int addr, int size, char *buf)
{
    int phy_addr=addr;
    enum w25qxx_status_t res = w25qxx_read_data(phy_addr, buf, size,W25QXX_QUAD);
	#if open_fs_debug
    printf("flash read addr:%x size:%d buf_head:%x %x\n",phy_addr,size,buf[0],buf[1]);
	#endif
    if (res != W25QXX_OK) {
		#if open_fs_debug
        printf("spifalsh read err\n");
		#endif
        return SPIFFS_ERR_FULL;
    }
    return SPIFFS_OK;
}
s32_t k210_sf_write(int addr, int size, char *buf)
{
    int phy_addr=addr;
    
    enum w25qxx_status_t res = w25qxx_write_data(phy_addr, buf, size);
	#if open_fs_debug
    printf("flash write addr:%x size:%d buf_head:%x,%x\n",phy_addr,size,buf[0],buf[1]);
	#endif
    if (res != W25QXX_OK) {
		#if open_fs_debug
        printf("spifalsh write err\n");
		#endif
        return SPIFFS_ERR_FULL;
    }
    return SPIFFS_OK;
}
s32_t k210_sf_erase(int addr, int size)
{
    int phy_addr=addr;
    unsigned char *temp_pool;
	#if open_fs_debug
    printf("flash erase addr:%x size:%f\n",phy_addr,size/1024.00);
	#endif
    enum w25qxx_status_t res = w25qxx_32k_block_erase(phy_addr);
    if (res != W25QXX_OK) {
		#if open_fs_debug
        printf("spifalsh erase err\n");
		#endif
        return SPIFFS_ERR_FULL;
    }
    return SPIFFS_OK;
}

void test_lock(spiffs *fs) {
}

void test_unlock(spiffs *fs) {
}

uint8_t init_py_file[]={
0x69,0x6d,0x70,0x6f,0x72,0x74,0x20,0x75,0x6f,0x73,0x0a,0x69,0x6d,0x70,0x6f,0x72,
0x74,0x20,0x6f,0x73,0x0a,0x69,0x6d,0x70,0x6f,0x72,0x74,0x20,0x6d,0x61,0x63,0x68,
0x69,0x6e,0x65,0x0a,0x69,0x6d,0x70,0x6f,0x72,0x74,0x20,0x63,0x6f,0x6d,0x6d,0x6f,
0x6e,0x0a,0x70,0x69,0x6e,0x5f,0x69,0x6e,0x69,0x74,0x3d,0x63,0x6f,0x6d,0x6d,0x6f,
0x6e,0x2e,0x70,0x69,0x6e,0x5f,0x69,0x6e,0x69,0x74,0x28,0x29,0x0a,0x70,0x69,0x6e,
0x5f,0x69,0x6e,0x69,0x74,0x2e,0x69,0x6e,0x69,0x74,0x28,0x29,0x0a,0x74,0x65,0x73,
0x74,0x5f,0x67,0x70,0x69,0x6f,0x5f,0x70,0x69,0x6e,0x5f,0x6e,0x75,0x6d,0x3d,0x31,
0x35,0x0a,0x66,0x70,0x69,0x6f,0x61,0x3d,0x6d,0x61,0x63,0x68,0x69,0x6e,0x65,0x2e,
0x66,0x70,0x69,0x6f,0x61,0x28,0x29,0x0a,0x66,0x70,0x69,0x6f,0x61,0x2e,0x73,0x65,
0x74,0x5f,0x66,0x75,0x6e,0x63,0x74,0x69,0x6f,0x6e,0x28,0x74,0x65,0x73,0x74,0x5f,
0x67,0x70,0x69,0x6f,0x5f,0x70,0x69,0x6e,0x5f,0x6e,0x75,0x6d,0x2c,0x36,0x33,0x29,
0x0a,0x74,0x65,0x73,0x74,0x5f,0x70,0x69,0x6e,0x3d,0x6d,0x61,0x63,0x68,0x69,0x6e,
0x65,0x2e,0x70,0x69,0x6e,0x28,0x37,0x2c,0x32,0x2c,0x30,0x29,0x0a,0x6c,0x63,0x64,
0x3d,0x6d,0x61,0x63,0x68,0x69,0x6e,0x65,0x2e,0x73,0x74,0x37,0x37,0x38,0x39,0x28,
0x29,0x0a,0x6c,0x63,0x64,0x2e,0x69,0x6e,0x69,0x74,0x28,0x29,0x0a,0x6c,0x63,0x64,
0x2e,0x64,0x72,0x61,0x77,0x5f,0x73,0x74,0x72,0x69,0x6e,0x67,0x28,0x31,0x31,0x36,
0x2c,0x31,0x32,0x31,0x2c,0x22,0x57,0x65,0x6c,0x63,0x6f,0x6d,0x65,0x20,0x74,0x6f,
0x20,0x4d,0x61,0x69,0x78,0x50,0x79,0x22,0x29,0x0a,0x69,0x66,0x20,0x74,0x65,0x73,
0x74,0x5f,0x70,0x69,0x6e,0x2e,0x76,0x61,0x6c,0x75,0x65,0x28,0x29,0x20,0x3d,0x3d,
0x20,0x30,0x3a,0x0a,0x20,0x20,0x20,0x20,0x70,0x72,0x69,0x6e,0x74,0x28,0x27,0x74,
0x65,0x73,0x74,0x27,0x29,0x0a,0x20,0x20,0x20,0x20,0x6d,0x61,0x63,0x68,0x69,0x6e,
0x65,0x2e,0x74,0x65,0x73,0x74,0x28,0x29,0x0a};


void my_spiffs_init(){

	spiffs_config cfg;
	cfg.phys_size = SPIFFS_CFG_PHYS_SZ(fs); // use all spi flash
	cfg.phys_addr = SPIFFS_CFG_PHYS_ADDR(fs); // start spiffs at start of spi flash
	cfg.phys_erase_block = SPIFFS_CFG_PHYS_ERASE_SZ(fs); // according to datasheet
	cfg.log_block_size = SPIFFS_CFG_LOG_BLOCK_SZ(fs); // let us not complicate things
	cfg.log_page_size = SPIFFS_CFG_LOG_PAGE_SZ(fs); // as we said

	cfg.hal_read_f = k210_sf_read;
	cfg.hal_write_f = k210_sf_write;
	cfg.hal_erase_f = k210_sf_erase;

	int res = SPIFFS_mount(&fs,
						   &cfg,
						   spiffs_work_buf,
						   spiffs_fds,
						   sizeof(spiffs_fds),
					       spiffs_cache_buf,
						   sizeof(spiffs_cache_buf),
						   0);
	printf("[MAIXPY]:Spiffs Mount %s \n", res?"failed":"successful");
	if(foce_format_fs || res != SPIFFS_OK || res==SPIFFS_ERR_NOT_A_FS)
	{
		SPIFFS_unmount(&fs);printf("[MAIXPY]:Spiffs Unmount.\n");
		printf("[MAIXPY]:Spiffs Formating...\n");
		s32_t format_res=SPIFFS_format(&fs);
		printf("[MAIXPY]:Spiffs Format %s \n",format_res?"failed":"successful");
		res = SPIFFS_mount(&fs,
			&cfg,
			spiffs_work_buf,
			spiffs_fds,
			sizeof(spiffs_fds),
			spiffs_cache_buf,
			sizeof(spiffs_cache_buf),
			0);
		printf("[MAIXPY]:Spiffs Mount %s \n", res?"failed":"successful");
		if(!res)
		{
			printf("[MAIXPY]:Spiffs Write init file %s \n");
			spiffs_file fd;
			fd=SPIFFS_open(&fs,"/init.py", SPIFFS_CREAT | SPIFFS_TRUNC | SPIFFS_RDWR, 0);
			if(fd != -1){
				s32_t ls_res = SPIFFS_lseek(&fs, fd,0,0);
				if(!ls_res){
					s32_t w_res = SPIFFS_write(&fs, fd,init_py_file,sizeof(init_py_file));
					if(w_res <= 0){
					}else{
						s32_t f_res = SPIFFS_fflush(&fs, fd);
					}
				}
			}
			SPIFFS_close (&fs, fd);
		}

	}
	
	return;

}

int format_fs(void)
{
	spiffs_config cfg;
	cfg.phys_size = SPIFFS_CFG_PHYS_SZ(fs); // use all spi flash
	cfg.phys_addr = SPIFFS_CFG_PHYS_ADDR(fs); // start spiffs at start of spi flash
	cfg.phys_erase_block = SPIFFS_CFG_PHYS_ERASE_SZ(fs); // according to datasheet
	cfg.log_block_size = SPIFFS_CFG_LOG_BLOCK_SZ(fs); // let us not complicate things
	cfg.log_page_size = SPIFFS_CFG_LOG_PAGE_SZ(fs); // as we said

	cfg.hal_read_f = k210_sf_read;
	cfg.hal_write_f = k210_sf_write;
	cfg.hal_erase_f = k210_sf_erase;

	SPIFFS_unmount(&fs);SPIFFS_unmount(&fs);printf("[MAIXPY]:Spiffs Unmount.\n");
	printf("[MAIXPY]:Spiffs Formating...\n");
	s32_t format_res=SPIFFS_format(&fs);
	printf("[MAIXPY]:Spiffs Format %s \n",format_res?"failed":"successful");
//	printf("spiffs formated end \n");
//	printf("w25qxx_page_program_fun addr %p\n",w25qxx_page_program_fun);
//	printf("spiffs formated \n");
	int res = SPIFFS_mount( &fs,
						&cfg,
						spiffs_work_buf,
						spiffs_fds,
						sizeof(spiffs_fds),
						spiffs_cache_buf,
						sizeof(spiffs_cache_buf),
						0);
	printf("[MAIXPY]:Spiffs Mount %s \n", res?"failed":"successful");
	return res;
}
