sudo docker run -d \
  -p 80:8080 \
  -v /home/alex/Documents/homer:/www/assets \
  --restart=always \
  b4bz/homer:latest \