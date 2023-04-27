for (($i = 255); $i -gt 223; $i--)
{
	Write-Host -NoNewline "$i "
	D:\Programy\hoot\hootclient volume $i
	Start-Sleep -Milliseconds 80
}

for (($i = 223); $i -gt 191; $i--)
{
	Write-Host -NoNewline "$i "
	D:\Programy\hoot\hootclient volume $i
	Start-Sleep -Milliseconds 40
}

for (($i = 191); $i -gt 127; $i--)
{
	Write-Host -NoNewline "$i "
	D:\Programy\hoot\hootclient volume $i
	Start-Sleep -Milliseconds 20
}

for (($i = 127); $i -gt 63; $i--)
{
	Write-Host -NoNewline "$i "
	D:\Programy\hoot\hootclient volume $i
	Start-Sleep -Milliseconds 20
}

for (($i = 63); $i -gt 31; $i--)
{
	Write-Host -NoNewline "$i "
	D:\Programy\hoot\hootclient volume $i
	Start-Sleep -Milliseconds 40
}

for (($i = 31); $i -gt -1; $i--)
{
	Write-Host -NoNewline "$i "
	D:\Programy\hoot\hootclient volume $i
	Start-Sleep -Milliseconds 80
}