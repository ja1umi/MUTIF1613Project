#!/bin/bash
rsync -avz -e "ssh -p 443" ykasumi@cloudbase.mercurynb.org:/home/ykasumi/projects/CELESTE/ .
