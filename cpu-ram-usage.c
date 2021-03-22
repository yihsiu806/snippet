#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <getopt.h>
#include <sys/sysinfo.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <ctype.h>

#define DEFAULT_IP     "192.168.88.51"
#define DEFAULT_PORT   7777
#define DEFAULT_TARGET 7053

#define max(m,n) ((m) > (n) ? (m) : (n))
#define min(m,n) ((m) < (n) ? (m) : (n))

const char *program_name;
char pid_path[128];
char pid_vm_path[128];

struct cpust {
    unsigned long utime;
    unsigned long stime;
    unsigned long user;
    unsigned long nice;
    unsigned long system;
    unsigned long idle;
    unsigned long iowait;
    unsigned long irq;
    unsigned long softirq;
    unsigned long steal;
    unsigned long guest;
    unsigned long guest_nice;
    unsigned long vmsize;
    unsigned long vmpeak;
    unsigned long vmrss;
    unsigned long vmhwm;
    unsigned long fps1;
    unsigned long fps2;
    unsigned long fps3;
    unsigned long fps4;
};

const struct option long_options[] = {
    { "ip",     1, NULL, 'i' },
    { "port",   1, NULL, 'p' },
    { "target", 1, NULL, 't' },
    { "help",   1, NULL, 'h' },
    { NULL,     0, NULL, 0 }
};

void read_pid_vm(struct cpust *st)
{
    FILE *fp;
    unsigned long vmsize = 0, vmpeak = 0, vmrss = 0, vmhwm = 0;
    char line[128];
    char *token;

    fp = fopen(pid_vm_path, "r");

    if (fp == NULL) {
        perror("pid_vm_path");
        exit(1);
    }

    while (!vmsize || !vmpeak || !vmrss || !vmhwm) {
        if (!fgets(line, sizeof(line), fp)) {
            break;
        }

        if (strncmp(line, "VmPeak:", 7) == 0) {
            token = strtok(line, " ");
            while (token) {
                if (isdigit(token[0])) {
                    vmpeak = strtoll(token, NULL, 10);
                    break;
                }
                token = strtok(NULL, " ");
            }
        }

        if (strncmp(line, "VmSize:", 7) == 0) {
            token = strtok(line, " ");
            while (token) {
                if (isdigit(token[0])) {
                    vmsize = strtoll(token, NULL, 10);
                    break;
                }
                token = strtok(NULL, " ");
            }
        }

        if (strncmp(line, "VmHWM:", 6) == 0) {
            token = strtok(line, " ");
            while (token) {
                if (isdigit(token[0])) {
                    vmhwm = strtoll(token, NULL, 10);
                    break;
                }
                token = strtok(NULL, " ");
            }
        }

        if (strncmp(line, "VmRSS:", 6) == 0) {
            token = strtok(line, " ");
            while (token) {
                if (isdigit(token[0])) {
                    vmrss = strtoll(token, NULL, 10);
                    break;
                }
                token = strtok(NULL, " ");
            }
        }
    }

    st->vmsize = vmsize;
    st->vmpeak = vmpeak;
    st->vmrss = vmrss;
    st->vmhwm = vmhwm;

    fclose(fp);
}

void read_pid_st(struct cpust *st)
{
    FILE *fp;

    fp = fopen(pid_path, "r");

    if (fp == NULL) {
        perror("pid_path");
        exit(1);
    }

    if (fscanf(fp, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu", &(st->utime), &(st->stime)) == EOF)
        perror("fscanf");

    fclose(fp);
}

void read_st(struct cpust *st)
{
    FILE *fp;

    fp = fopen("/proc/stat", "r");

    if (fp == NULL) {
        perror("/proc/stat");
        exit(1);
    }

    if (fscanf(fp, "%*s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu", &(st->user), &(st->nice), &(st->system), &(st->idle), &(st->iowait), &(st->irq), &(st->softirq), &(st->steal), &(st->guest), &(st->guest_nice)) == EOF)
        perror("fscanf");

    fclose(fp);
}

void print_usage(void)
{
    printf("Usage: %s -i 192.168.0.1 -p 9999 -t 7053\n", program_name);
    printf("  -i --ip     remote parser's ip address\n"
           "  -p --port   remote parser's port\n"
           "  -t --target target process pid\n");
    exit(1);
}

int main(int argc, char *argv[])
{

    int opt;
    char *ip;
    short port, target;
    int sockfd;
    struct sockaddr_in addr;
    struct sysinfo info;
    struct cpust st1, st2;
    unsigned long elapse_cpu_all;
    unsigned long elapse_cpu_process;
    double cpu, maxcpu = 0, mincpu = 0;
    long double cputotal = 0;
    unsigned long count = 0;
    unsigned long vmsize_total = 0, vmrss_total = 0;
    char msg[1024];

    program_name = argv[0];

    ip = DEFAULT_IP;
    port = DEFAULT_PORT;
    target = DEFAULT_TARGET;

    while ((opt = getopt_long(argc, argv, "i:p:t:h", long_options, NULL)) != -1) {
        switch (opt) {
        case 'i':
            ip = optarg;
            break;
        case 'p':
            port = atoi(optarg);
            break;
        case 't':
            target = atoi(optarg);
            break;
        case 'h':
            print_usage();
        default:
            print_usage();
        }
    }

    // setup path
    memset(pid_path, 0, sizeof(pid_path));
    snprintf(pid_path, sizeof(pid_path), "/proc/%d/stat", target);

    memset(pid_vm_path, 0, sizeof(pid_vm_path));
    snprintf(pid_vm_path, sizeof(pid_vm_path), "/proc/%d/status", target);

    if (access(pid_path, F_OK) < 0 || access(pid_vm_path, F_OK) < 0) {
        perror("pid does not exist");
        exit(1);
    }

    // setup socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    // get total ram
    memset(&info, 0, sizeof(info));
    if (sysinfo(&info) < 0) {
        perror("sysinfo");
        exit(1);
    }

    memset(&st1, 0, sizeof(st1));
    memset(&st2, 0, sizeof(st1));

    read_pid_st(&st1);
    read_st(&st1);

    for (;;) {
        sleep(1);
        read_pid_st(&st2);
        read_st(&st2);

        read_pid_vm(&st2);

        //read_fps(&st2);

        count++;

        elapse_cpu_all = (st2.user + st2.nice + st2.system + st2.idle + st2.iowait + st2.irq + st2.softirq + st2.steal + st2.guest + st2.guest_nice) - (st1.user + st1.nice + st1.system + st1.idle + st1.iowait + st1.irq + st1.softirq + st1.steal + st1.guest + st1.guest_nice);
        elapse_cpu_process = (st2.utime + st2.stime) - (st1.utime + st1.stime);

        cpu = (double)elapse_cpu_process / elapse_cpu_all;
        cpu *= 100;
        cputotal += cpu;
        maxcpu = max(maxcpu, cpu);
        mincpu = min(mincpu, cpu);

        vmsize_total += st2.vmsize;
        vmrss_total += st2.vmrss;

        snprintf(msg, sizeof(msg), "%3.2f %ld %ld %ld %ld", cpu, st2.vmpeak, st2.vmsize, st2.vmhwm, st2.vmrss);
        sendto(sockfd, msg, strlen(msg), 0, (struct sockaddr*)&addr, sizeof(addr));

        fprintf(stderr, "%s\n", msg);

        st1 = st2;
    }

    return 0;
}
