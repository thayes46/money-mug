FROM node:alpine

WORKDIR /opt/app
RUN chown -R node:node /opt/app
COPY --chown=node:node ./node/package*.json /opt/app/

USER node
RUN npm install

COPY --chown=node:node ./static/ ./public/static/
COPY --chown=node:node ./html/ ./views/

COPY --chown=node:node ./node/index.js ./

EXPOSE 8000
ENTRYPOINT ["node"]
CMD ["index.js"]
