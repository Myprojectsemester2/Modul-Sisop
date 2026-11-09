#!/bin/bash

# ambil 2 titik diagonal (baris 1 dan 3)
line1=$(sed -n '1p' titik-penting.txt)
line2=$(sed -n '3p' titik-penting.txt)

# parsing format: id, site_name, latitude, longitude
IFS=',' read id1 name1 lat1 lon1 <<< "$line1"
IFS=',' read id2 name2 lat2 lon2 <<< "$line2"

# bersihkan spasi
lat1=$(echo "$lat1" | xargs)
lon1=$(echo "$lon1" | xargs)
lat2=$(echo "$lat2" | xargs)
lon2=$(echo "$lon2" | xargs)

# validasi data
if [[ -z "$lat1" || -z "$lon1" || -z "$lat2" || -z "$lon2" ]]; then
    echo "Error: data tidak valid!"
    exit 1
fi

# hitung titik tengah diagonal
mid_lat=$(echo "scale=6; ($lat1 + $lat2) / 2" | bc)
mid_lon=$(echo "scale=6; ($lon1 + $lon2) / 2" | bc)

# tampilkan hasil
echo "Koordinat pusat: $mid_lat, $mid_lon"

# simpan ke file sesuai soal
echo "$mid_lat, $mid_lon" > posisipusaka.txt
