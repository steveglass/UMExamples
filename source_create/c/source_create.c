/* Code Disclaimer? */

#include <stdio.h>

#if defined(_MSC_VER)
/* Windows-only includes */
#include <winsock2.h>
#define SLEEP(s) Sleep((s)*1000)
#else
/* Unix-only includes */
#include <stdlib.h>
#include <unistd.h>
#define SLEEP(s) sleep(s)
#endif

#include <lbm/lbm.h>

main()
{
        lbm_context_t *ctx;                     /* Context object */
        lbm_topic_t *topic;                     /* Topic object */
        lbm_src_t *src;                         /* Source object */
        lbm_src_topic_attr_t * tattr;           /* Source topic attribute object */
        lbm_context_attr_t * cattr;             /* Context attribute object */
        int err;                                /* Used for checking API return codes */

#if defined(_WIN32)
        /* windows-specific code */
        WSADATA wsadata;
        int wsStat = WSAStartup(MAKEWORD(2,2), &wsadata);
        if (wsStat != 0)
        {
                printf("line %d: wsStat=%d\n",__LINE__,wsStat);
                exit(1);
        }
#endif

        if (lbm_context_attr_create(&cattr) != 0)
        {
                fprintf(stderr, "lbm_context_attr_create: %s\n", lbm_errmsg());
                exit(1);
        }

        err = lbm_context_create(&ctx, cattr, NULL, NULL);
        if (err)
        {
                printf("line %d: %s\n", __LINE__, lbm_errmsg());
                exit(1);
        }

        if (lbm_src_topic_attr_create(&tattr) != 0)
        {
                fprintf(stderr, "lbm_src_topic_attr_create: %s\n", lbm_errmsg());
                exit(1);
        }

        err = lbm_src_topic_alloc(&topic, ctx, "test.topic", tattr);
        if (err)
        {
                printf("lbm_src_topic_alloc - line %d: %s\n", __LINE__, lbm_errmsg());
                exit(1);
        }

	err = lbm_src_topic_attr_delete(tattr);
	if (err)
        {
                printf("lbm_src_topic_attr_delete - line %d: %s\n", __LINE__, lbm_errmsg());
                exit(1);
        }

        err = lbm_src_create(&src, ctx, topic, NULL, NULL, NULL);
        if (err)
        {
                printf("lbm_src_create - line %d: %s\n", __LINE__, lbm_errmsg());
                exit(1);
        }

	err = lbm_src_delete(src);
	if (err)
	{
		printf("lbm_src_delete - line %d: %s\n", __LINE__, lbm_errmsg());
		exit(1);
	}

#if defined(_MSC_VER)
	WSACleanup();
#endif
}

