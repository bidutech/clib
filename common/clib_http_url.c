/*************************************************************************
	> File Name: clib_http_url.c
	> Author: shanhai2015
	> Mail: shanhai2015
	> Created Time: 2017年02月22日 星期三 17时12分36秒
 ************************************************************************/

#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <assert.h>
#include <ctype.h>
#include "clib_http_url.h"
#include "clib_string.h"

//#ifdef _LINUX
#include <ifaddrs.h>
//#endif

static char *strncasestr(char *str, const char *sub)
{
  int len = strlen(str);
  int slen= strlen(sub);
  int i   = 0;

  for(; i <= len - slen; ++i) {
      if(strncasecmp(str+i, sub, slen) == 0) {
          return  str + i;
        }
    }

  return  NULL;
}

void clib_http_url_string_append(char *str, unsigned int len, const char *fmt, ...)
{
  va_list args;
  char    tmp[65536];

  memset(tmp, 0, sizeof(tmp));

  va_start(args, fmt);
  vsprintf(tmp, fmt, args);
  va_end(args);

  if (strlen(tmp) + strlen(str) >= len) {
      strncat(str, tmp, len - strlen(str) - 1);
    } else {
      strncat(str, tmp, strlen(tmp));
    }
}

int clib_http_url_get_value(const char *src, const char *varname, char *buf, int blen)
{
  const char  separatesymbol = '&';
  const char  assignsymbol   = '=';
  const char  startsymbol    = '?';
  const char  quot = '\'';
  const char *pos = src;
  int varnamelen  = strlen(varname);
  int varlen = 0;
  int ret    = 0;

  if (!src || !varname || !buf || blen <= 0) return -1;
  buf[0] = '\0';

  for(;(pos = strncasestr((char *)pos, varname)); pos += varnamelen)
    {
      if( *(pos + varnamelen) == assignsymbol &&
          (pos == src || *(pos - 1) == separatesymbol || *(pos - 1) == startsymbol))
        {
          if (*(pos + varnamelen + 1) && *(pos + varnamelen + 1) == quot) {
              pos++;
              for(;(varlen < blen) && *(pos + varnamelen + varlen + 1) &&
                  *(pos + varnamelen + varlen + 1) != quot; varlen++)
                {
                  *(buf + varlen) = *(pos + varnamelen + varlen + 1);
                }
              *(buf + varlen) = 0;
              goto exit;
            }
          for(;(varlen < blen) && *(pos + varnamelen + varlen + 1) &&
              *(pos + varnamelen + varlen + 1) != separatesymbol; ++varlen)
            {
              *(buf + varlen) = *(pos + varnamelen + varlen + 1);
            }
          *(buf + varlen) = 0;
          goto exit;
        }

    }
  ret = -1;

exit:
  return ret;
}

int clib_http_url_get_uint(const char *src, const char *key, unsigned int *val)
{
  int  ret  = 0;
  long lint = 0;
  char tmp[32] = {0};

  ret = clib_http_url_get_value(src, key, tmp, sizeof(tmp));
  if(ret < 0){
      return  ret;
    }
  lint = atol(tmp);
  if(lint < 0){
      return  -1;
    }
  *val = lint;

  return  0;
}

int clib_http_url_get_int(const char *src, const char *key, int *val)
{
  int  ret  = 0;
  int  lint = 0;
  char tmp[32] = {0};

  ret = clib_http_url_get_value(src, key, tmp, sizeof(tmp));
  if(ret < 0){
      return  ret;
    }
  lint = atoi(tmp);
  if(lint < 0){
      return -1;
    }
  *val = lint;

  return  0;
}

int clib_http_url_get_uint64(const char *src, const char *key, uint64_t *val)
{
  int ret = 0;
  char tmp[32]   = {0};

  ret = clib_http_url_get_value(src, key, tmp, sizeof(tmp));
  if(ret  < 0){
      return  ret;
    }
  *val = strtoull(tmp, NULL, 10);

  return  0;
}

int clib_http_url_set_value(const char *varname, const char *varvalue, char *obj, int olen)
{
  int ret = 0;
  const char separatesymbol = '&';
  const char assignsymbol   = '=';
  const char startsymbol    = '?';
  char      *pos            = obj;
  int varnamelen = strlen(varname);
  int replacelen = 0;
  int valuelen   = strlen(varvalue);
  int movdist    = 0;
  int objlen     = strlen(obj);
  int i = 0, j   = 0;

  if (!varname || !varvalue || !obj || olen <= 0) return -1;

  for(; (pos = strncasestr((char *)pos, varname)) != 0; pos += varnamelen)
    {
      if( *(pos + varnamelen) == assignsymbol &&
          (pos == obj || *(pos - 1) == separatesymbol || *(pos - 1) == startsymbol))
        {
          i = pos + varnamelen - obj;
          for(; obj[i] && obj[i] != separatesymbol && i < olen; ++i);
          if( i == olen ) {
              return -1;
            }

          replacelen = obj + i - (pos + varnamelen + 1);
          movdist = valuelen - replacelen;
          if(movdist > 0) {
              for(j = objlen; j >= i ; --j) {
                  obj[j + movdist] = obj[j];
                }
            } else if(movdist < 0) {
              for(j = i; j <= objlen  ; ++j) {
                  obj[j + movdist] = obj[j];
                }
            }
          memcpy(pos + varnamelen + 1, varvalue, valuelen);
          goto exit;
        } else {
          continue;
        }
    }
  if(strlen(obj) + varnamelen + strlen(varvalue) + 2 > (unsigned int)olen) {
      return -1;
    }
  strcat(obj, "&");
  strcat(obj, varname);
  strcat(obj, "=");
  strcat(obj, varvalue);

exit:
  return ret;
}

#ifdef _LINUX
static int CheckIpLegal(const char *ip_addr)
{
  long long a, b, c, d;
  const char *start = NULL;
  char *end = NULL;

  if (!ip_addr) return -1;
  start = ip_addr;

  a = strtol(start, &end, 10);
  if (a >= 256) goto exit;
  if (!end || end[0] != '.') goto exit;
  start = end + 1;
  b = strtol(start, &end, 10);
  if (b >= 256) goto exit;
  if (!end || end[0] != '.') goto exit;
  start = end + 1;
  c = strtol(start, &end, 10);
  if (c >= 256) goto exit;
  if (!end || end[0] != '.') goto exit;
  start = end + 1;
  d = strtol(start, &end, 10);
  if (d >= 256) goto exit;

  if (!a && !b && !c && !d) {
      return -2;
    }

  return 0;

exit:
  return -3;
}

int local_ip(int family, char *buf, int len)
{
  struct ifaddrs *ifaddr, *ifa;
  int family2, s;
  char host[NI_MAXHOST];

  if (getifaddrs(&ifaddr) == -1) {
      return -1;
    }

  for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
      if (ifa->ifa_addr == NULL)
        continue;

      family2 = ifa->ifa_addr->sa_family;
#ifdef _DEBUG
      printf("%s address family: %d%s\n",
             ifa->ifa_name, family2,
             (family2 == AF_PACKET) ? " (AF_PACKET)" :
                                      (family2 == AF_INET) ?   " (AF_INET)" :
                                                               (family2 == AF_INET6) ?  " (AF_INET6)" : "");
#endif

      if (family2 == AF_INET || family2 == AF_INET6) {
          s = getnameinfo(ifa->ifa_addr,
                          (family2 == AF_INET) ? sizeof(struct sockaddr_in) :
                                                 sizeof(struct sockaddr_in6),
                          host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
          if (s != 0) {
              freeifaddrs(ifaddr);
              return -2;
            }
          if (family == AF_INET && CheckIpLegal(host)) {
              continue;
            }
          if (family == family2 && strcmp(host, "127.0.0.1")) {
              if (len > NI_MAXHOST) {
                  len = NI_MAXHOST;
                }
              strncpy(buf, host, len);
              break;
            }
#ifdef _DEBUG
          printf("\taddress: <%s>:%d\n", host, CheckIpLegal(host));
#endif
        }
    }
  freeifaddrs(ifaddr);

  return 0;
}
#endif

static int get_ifacename(char *iface_name, size_t size)
{
  int ret;
  int flgs, ref, use, metric, mtu, win,ir;
  unsigned long d, g, m;
  char  devname[24];
  FILE *fp;

  if ((fp = fopen("/proc/net/route", "r")) == NULL) {
      return -1;
    }
  if (fscanf(fp, "%*[^\n]\n") < 0) {
      fclose(fp);
      return -2;
    }
  while (1) {
      ret = fscanf(fp, "%19s%lx%lx%X%d%d%d%lx%d%d%d\n",
                   devname, &d, &g, &flgs, &ref, &use,
                   &metric, &m, &mtu, &win, &ir);
      if (11 != ret) {
          if ((ret < 0) && feof(fp)) {
              break;
            }
          continue;
        }
      strncpy(iface_name, devname, size);
      fclose(fp);
      return 0;
    }

  fclose(fp);
  return -3;
}

static int ip_address(char *iface_name, char *ip, size_t size)
{
  int fd;
  struct ifreq ifr;
  struct sockaddr_in addr;

  memset(&ifr, 0, sizeof(struct ifreq));
  strcpy(ifr.ifr_name, iface_name);

  if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      return -1;
    }
  if (ioctl(fd, SIOCGIFADDR, &ifr)) {
      close(fd);
      return -2;
    }
  memcpy(&addr, &ifr.ifr_addr, sizeof(addr));
  strncpy(ip, inet_ntoa(addr.sin_addr), size);
  close(fd);

  return 0;
}

int clib_http_url_local_ip(char *buf, int len)
{
  char iface_name[50];

  get_ifacename(iface_name, sizeof(iface_name));
  ip_address(iface_name, buf, len);

  return 0;
}

int clib_http_url_parse_xml(const char *xml, char *key, char *val, int size)
{
#define KEY_SIZE 32
#define BUF_SIZE 1024
  short i;
  char keyh[KEY_SIZE] = {0}, keyt[KEY_SIZE] = {0}, buf[BUF_SIZE] = {0}, *p, *q;

  if(strlen(key) > KEY_SIZE-4)
    return -1;
  bzero(val, size);
  sprintf(keyh, "<%s>", key);
  sprintf(keyt, "</%s>", key);
  if(!(p = strstr(xml, keyh)))
    return -2;
  p += strlen(keyh);
  if(!(q = strstr(p, keyt)))
    return -3;
  if(q-p >= BUF_SIZE)
    return -4;
  for(i=0; p+i!=q; i++)
    buf[i] = *(p+i);
  if((p = strstr(buf, "<![CDATA[")))
    {
      p += strlen("<![CDATA[");
      if(!(q = strstr(p, "]]>")))
        return -5;
      if(q-p >= size)
        return -6;
      for(i=0; p+i!=q; i++)
        val[i] = *(p+i);
      return 0;
    }
  strcpy(val, buf);
  return 0;
}

char *clib_http_url_parse_ip(uint32_t uip, char* buf, int len)
{
  if(!inet_ntop(AF_INET, &uip, buf, len))
    {
      return NULL;
    }
  return buf;
}

char *clib_http_url_strrstr(char *s1, char *s2)
{
  char *last = NULL;
  char *current = NULL;

  if (*s2 != '\0')
    {
      current = strstr(s1, s2);
      while (current != NULL)
        {
          last = current;
          current = strstr(last+1, s2);
        }
    }

  return last;
}

static int util_htoi(char *s)
{
  int value;
  int c;

  c = ((unsigned char *)s)[0];
  if (isupper(c))
    c = tolower(c);
  value = (c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10) * 16;

  c = ((unsigned char *)s)[1];
  if (isupper(c))
    c = tolower(c);
  value += c >= '0' && c <= '9' ? c - '0' : c - 'a' + 10;

  return (value);
}

char *clib_http_url_decode(char *str)
{
  char *start = str;
  char *dest  = str;
  char *data  = str;
  size_t len = strlen(str);

  while (len--)
    {
      if (*data == '+')
        {
          //*dest = ' ';
        }
      else if (*data == '%' && len >= 2 && isxdigit((int) *(data + 1)) && isxdigit((int) *(data + 2)))
        {
          *dest = (char) util_htoi(data + 1);
          data += 2;
          len -= 2;
        }
      else
        {
          *dest = *data;
        }
      data++;
      dest++;
    }
  *dest = '\0';
  return start;
}

char *clib_http_url_encode(const char *src, char *dest, size_t ulen)
{
  size_t pos = 0;
  size_t dlen = 0;
  size_t srclen = strlen(src);

  assert(src && dest);
  for(; pos < srclen; ++pos)
    {
      switch(src[pos])
        {
        case '&':
          /* replace with &amp */
          if(ulen < dlen + 4 + 1)  return NULL;
          dest[dlen] = '&',++dlen;
          dest[dlen] = 'a',++dlen;
          dest[dlen] = 'm',++dlen;
          dest[dlen] = 'p',++dlen;
          break;
        case '\"':
          /* replace with &quot */
          if(ulen < dlen + 5 +1)  return NULL;
          dest[dlen] = '&',++dlen;
          dest[dlen] = 'q',++dlen;
          dest[dlen] = 'u',++dlen;
          dest[dlen] = 'o',++dlen;
          dest[dlen] = 't',++dlen;
          break;
        case '\'':
          /* replace with &#039 */
          if(ulen < dlen + 5 +1)  return NULL;
          dest[dlen] = '&',++dlen;
          dest[dlen] = '#',++dlen;
          dest[dlen] = '0',++dlen;
          dest[dlen] = '3',++dlen;
          dest[dlen] = '9',++dlen;
          break;
        case '<':
          /* replace with &lt */
          if(ulen < dlen + 3 +1)  return NULL;
          dest[dlen] = '&',++dlen;
          dest[dlen] = 'l',++dlen;
          dest[dlen] = 't',++dlen;
          break;
        case '>':
          /* replace with &gt */
          if(ulen < dlen + 3 +1)  return NULL;
          dest[dlen] = '&',++dlen;
          dest[dlen] = 'g',++dlen;
          dest[dlen] = 't',++dlen;
          break;
        default:
          dest[dlen] = src[pos];
          ++dlen;
          break;
        }
    }
  dest[dlen] = 0;

  return dest;
}

void clib_http_url_trim(char *buf)
{
  int i = 0, len = 0;
  char *p = NULL;

  if (!buf) return;

  len = strlen(buf);
  while(len > i) {
      if (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\r' || buf[i] == '\n')
        i++;
      else
        break;
    }

  memmove(buf, buf + i, len - i);
  len = len - i;
  buf[len] = 0;

  if (len == 0) return;

  i = len - 1;
  while (i > 0) {
      if (buf[i] == ' ' || buf[i] == '\t' || buf[i] == '\r' || buf[i] == '\n') {
          i--;
        } else {
          break;
        }
    }
  buf[i + 1] = 0;

  p = strstr(buf, "#");
  if (p) *p = 0;
}

int
clib_http_url_from_header(char * httpheader, char * buf, size_t bufsize)
{
  return clib_string_get_midstr_between_space(httpheader, buf, bufsize);
}

int
clib_http_url_get_host_port(char *url,char *host,char *port)
{
    char *ip = host;
  char *p = NULL;
  int offset = 0;
  char DOMAIN_NAME[128];
  p = strstr(url,"http://");

  if(p == NULL)
    {
      offset = 0;
    }
  else
    {
      offset = strlen("http://");
    }
  p = strchr(url+offset,'/');
  if(p == NULL)    {
#ifdef DEBUG_HTTP
      printf("url:%s format error\n",url);
#endif
      return -1;
    }
  else
    {
      memset(DOMAIN_NAME,0x0,sizeof(DOMAIN_NAME));
      memcpy(DOMAIN_NAME,url+offset,(p-url-offset));
      p = strchr(DOMAIN_NAME,':');
      if(p == NULL)
        {
          strcpy(ip,DOMAIN_NAME);
          strcpy(port,"80");
          return 1;
        }
      else
        {
          *p = '\0';
          strcpy(ip,DOMAIN_NAME);
          strcpy(port,p+1);
          return 2;
        }
      return 0;
    }
}
