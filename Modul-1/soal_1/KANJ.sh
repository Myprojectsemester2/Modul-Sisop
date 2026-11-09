BEGIN{
    FS=","
    option = ARGV[2]
    delete ARGV[2]
}

NR>1{
    total++

    carriage[$3] = 1

    age = $4
    sum_age += age

    if(age > max_age){
        max_age = age
        oldest = $2
    }

    if($5 == "Business"){
        business++
    }
}

END{

if(option=="a"){
    print "Jumlah seluruh penumpang KANJ adalah " total " orang"
}

else if(option=="b"){
    for(c in carriage){
        count++
    }
    print "Jumlah gerbong penumpang KANJ adalah " count
}

else if(option=="c"){
    print oldest " adalah penumpang kereta tertua dengan usia " max_age " tahun"
}

else if(option=="d"){
    avg = int(sum_age/total)
    print "Rata-rata usia penumpang adalah " avg " tahun"
}

else if(option=="e"){
    print "Jumlah penumpang business class ada " business " orang"
}

else{
    print "Soal tidak dikenali. Gunakan a, b, c, d, atau e."
}
}BEGIN { FS="," }

{
    print $1, $2, $3
}
