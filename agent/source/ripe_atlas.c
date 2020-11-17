#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <syscfg/syscfg.h>

#include <ripe_atlas.h>

void spawn_cmd(char* cmd, char* arg) {
	if (fork() == 0)
		execl(cmd, cmd, arg, NULL);
}

int _syscmd(char *cmd, char *retBuf, int retBufSize) {
    FILE *f;
    char *ptr = retBuf;
    int bufSize = retBufSize, bufbytes = 0, readbytes = 0;

    if ((f = popen(cmd, "r")) == NULL) {
        debug_printf("popen %s error\n", cmd);
        return -1;
    }

    while (!feof(f)) {
        *ptr = 0;
        if (bufSize >= 128) {
            bufbytes = 128;
        } else {
            bufbytes = bufSize - 1;
        }

        fgets(ptr, bufbytes, f);
        readbytes = strlen(ptr);
        if (readbytes == 0) break;
        bufSize -= readbytes;
        ptr += readbytes;
    }
    pclose(f);
    retBuf[retBufSize - 1] = 0;
    return 0;
}



bool ripeatlas_bootup()
{
	char buffer[8] = {0};

	bool enabled = false;
	bool schedule_enabled = false;

	int ret = syscfg_init();
	debug_printf("Init - ret = %d\n", ret);

	ret = syscfg_get(SYSCFG_NS, "Enable", buffer, 8); 

	debug_printf("Get - ret = %d\n", ret);
	
	if ( ret == 0)
		enabled = (strcmp(buffer, "true") == 0);


	if(enabled)
	{
		ripeatlas_set_enable(true);
	}
}

bool ripeatlas_get_enable()
{
	char cmd[] = "systemctl status atlas | grep 'Active' | sed 's/ *//' | cut -d' ' -f2";
	char out[16];

	_syscmd(cmd, out, sizeof(out));
	if ( strncmp("active", out, 6) == 0 )
	{
		return true;
	}
	return false;
}

void ripeatlas_set_enable(bool state)
{
	char cmd[50];
	char out[16];

	snprintf(cmd, sizeof(cmd), "systemctl %s atlas", state?"start":"stop");

	_syscmd(cmd, out, sizeof(out));

	int ret = 0;

	ret = syscfg_set(SYSCFG_NS, "Enable", state ? "true" : "false");
	debug_printf("Set - ret = %d\n", ret);
	ret = syscfg_commit();
	debug_printf("Commit - ret = %d\n", ret);
}

bool ripeatlas_get_status(char *status, unsigned int status_size)
{
	int ssh_pid = -1;
	bool connected = false;
	FILE *fd = fopen("/var/atlas-probe/status/con_keep_pid.vol", "r");

	if(fd == NULL)
	{
		connected = false;
	}
	else 
	{
		fscanf(fd, "%d", &ssh_pid);
		fclose(fd);
		fd=NULL;

		char ssh_info_path[32];
		sprintf(ssh_info_path, "/proc/%d/cmdline", ssh_pid);
		fd = fopen(ssh_info_path, "r");

		if(fd == NULL)
		{
			connected = false;
		}
		else
		{
			connected = true;
		}
	}
	strncpy(status, connected ? "Connected":"Disconnected", status_size );

	return true;
}


bool ripeatlas_get_key(char *key, unsigned int key_size)
{
	if( access( "/var/atlas-probe/etc/probe_key", F_OK ) != -1 ) 
	{
		// file  exist
		debug_printf("File probe_key exists\n", "");
		char cmd[] = "dropbearkey -y -f /var/atlas-probe/etc/probe_key | grep ssh-rsa | tr -d '\n'";
		char out[512];

		_syscmd(cmd, out, sizeof(out));
		
		strncpy(key, out, key_size);
	} else 
	{
		debug_printf("File probe_key doesn't exist\n","");
		strncpy(key, "none", key_size);
	}
	return true;
}
