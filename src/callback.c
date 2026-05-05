/******************************
* File			: callback.c
* Author		: Arjun A N S
* Date			: 2026-03-22
* Description		: Callback code for sysrepo 
******************************/

#include <sysrepo.h>
#include <stdio.h>
#include <unistd.h>

static int module_change_cb(sr_session_ctx_t *session, uint32_t sub_id,
                            const char *module_name, const char *xpath,
                            sr_event_t event, uint32_t request_id,
                            void *private_data)
{
    printf("\n🔥 CHANGE DETECTED in module: %s\n", module_name);

    sr_data_t *data = NULL;

    if (sr_get_data(session, "/simple-router:*", 0, 0, 0, &data) == SR_ERR_OK) {
        lyd_print_file(stdout, data->tree, LYD_XML, 0);
        sr_release_data(data);
    }

    return SR_ERR_OK;
}

int main() {
    sr_conn_ctx_t *conn = NULL;
    sr_session_ctx_t *sess = NULL;
    sr_subscription_ctx_t *sub = NULL;

    sr_connect(0, &conn);
    sr_session_start(conn, SR_DS_RUNNING, &sess);

    sr_module_change_subscribe(sess, "simple-router", NULL,
                                module_change_cb, NULL, 0, 0, &sub);

    printf("🚀 Listening for changes...\n");

    while (1) {
        sleep(1);
    }

    sr_disconnect(conn);
    return 0;
}

