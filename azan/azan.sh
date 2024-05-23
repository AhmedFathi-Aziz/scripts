#!/bin/bash

current_date=$(date +%d-%B-%Y)

url="https://api.aladhan.com/v1/timingsByCity/$current_date?city=Cairo&country=Arab+Rebuplic+Egypt"

response=$(curl -s "$url")

fajr=$(echo "$response" | jq -r '.data.timings.Fajr')
sunrise=$(echo "$response" | jq -r '.data.timings.Sunrise')
dhuhr=$(echo "$response" | jq -r '.data.timings.Dhuhr')
asr=$(echo "$response" | jq -r '.data.timings.Asr')
maghrib=$(echo "$response" | jq -r '.data.timings.Maghrib')
isha=$(echo "$response" | jq -r '.data.timings.Isha')

bold='\033[1m'  
green='\033[1;32m'
blue='\033[1;34m'
reset='\033[0m'

printf "%s\n" "-----------------------------------------"
printf "${bold}%-10s | %-10s${reset}\n" "Prayer" "Time"
printf "%s\n" "-----------------------------------------"
printf "${blue}%-10s | ${green}%s${reset}\n" "Fajr" "$fajr"
printf "${blue}%-10s | ${green}%s${reset}\n" "Sunrise" "$sunrise"
printf "${blue}%-10s | ${green}%s${reset}\n" "Dhuhr" "$dhuhr"
printf "${blue}%-10s | ${green}%s${reset}\n" "Asr" "$asr"
printf "${blue}%-10s | ${green}%s${reset}\n" "Maghrib" "$maghrib"
printf "${blue}%-10s | ${green}%s${reset}\n" "Isha" "$isha"
printf "%s\n" "-----------------------------------------"
printf "${bold}Current Date $current_date${reset}\n"