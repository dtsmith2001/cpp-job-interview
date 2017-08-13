# C++ Project for Job Interview

I was asked to do this project as part of a job interview in January 2017.

# Instructions

Download the following (big) file:

http://downloads.cms.gov/files/DE1_0_2008_to_2010_Carrier_Claims_Sample_1A.zip

The columns of interest in this file are:

- DESYNPUF_ID: to identify beneficiaries
- PRF_PHYSN_NPI_1: to identify providers

Each row corresponds to a different claim. The objective is to write a command-line-based application in C++ that reads this CSV file, counts the number of claims and unique beneficiaries per provider, and exports it to another CSV file.

As an example, the first 3 lines of the output look something like:

| PRF_PHYSN_NPI_1 | n_claims | n_beneficiaries |
|-----------------|----------|-----------------|
| 7128674558      | 20       | 13              |
| 9382129590      | 34       | 31              |

# Response

Build the program with build.sh instead of a makefile (no time for that).

Cut the columns required out of the big file using

```cut -f1,13 -d',' DE1_0_2008_to_2010_Carrier_Claims_Sample_1A.csv > columns.csv```

This preprocessing step saves time.

- To get the number of unique providers (498,018): ```tail -n +2 columns.csv | cut -f2 -d',' | sort -u | wc -l```
- To get the number of unique beneficiaries (49,278): ```tail -n +2 columns.csv | cut -f1 -d',' | sort -u | wc -l```

Validate claim count with

```grep 0000313693 columns.csv | wc -l```

and beneficiary count with

```grep 0000313693 columns.csv | cut -f1 -d',' | sort -u | wc -l```

See the script ```validate.sh``` to validate any provider id.

```
validate.sh <provider>
```
