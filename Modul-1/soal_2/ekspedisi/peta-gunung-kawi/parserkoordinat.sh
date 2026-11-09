#!/bin/bash

awk '
/"id":/ {
    match($0, /"id": *"([^"]+)"/, a)
    id = a[1]
}
/"site_name":/ {
    match($0, /"site_name": *"([^"]+)"/, a)
    site = a[1]
}
/"latitude":/ {
    match($0, /"latitude": *([-0-9.]+)/, a)
    lat = a[1]
}
/"longitude":/ {
    match($0, /"longitude": *([-0-9.]+)/, a)
    lon = a[1]

    print id ", " site ", " lat ", " lon
}
' gsxtrack.json > titik-penting.txt
