#!/bin/bash
# Dieses Skript im SVN Hauptverzeichnis ausführen
echo "--- UPDATE ---"; \
svn up && \
echo "--- BUILD ---"; \
mkdir -p ./Projekt/release && \
cd ./Projekt/release && \
cmake -DCMAKE_BUILD_TYPE=Release .. && \
make && \
echo "--- EXECUTE ---"; \
cd ../.. && \
./Projekt/release/Hockey # ausfuehren

