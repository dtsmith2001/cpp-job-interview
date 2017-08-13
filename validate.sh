#!/usr/bin/env bash

# there may be a better way to do it, but this works and is simple
n_claims=$(grep $1 columns.csv | wc -l)
n_beneficiaries=$(grep $1 columns.csv | cut -f1 -d',' | sort -u | wc -l)
echo "n_claims ${n_claims} n_beneficiaries ${n_beneficiaries}"
