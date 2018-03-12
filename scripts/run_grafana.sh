docker run \
    -d \
    -p 3000:3000 \
    --name=grafana \
    -e "GF_SERVER_ROOT_URL=http://localhost" \
    -e "GF_SECURITY_ADMIN_PASSWORD=secret" \
    -v "/var/lib/grafana:/var/lib/grafana" \
    grafana/grafana

