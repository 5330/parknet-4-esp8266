#  best i can find right now. they update it at :54 of every hour but it seems to change location in the table. pull 1017.9 field out only?!

 curl -s "http://www.wrh.noaa.gov/cnrfc/rsa_getObs.php?sid=KIND&num=48" |grep ":54" |head -1 |awk -F" " '{print $42}' |cut -b 8-13

#example output
# <tr class="smallmetar" align="center" valign="top" bgcolor="#f5f5f5"><td class="smallmetar" nowrap>25</td><td class="smallmetar" nowrap align="center">11:54am</td><td class="smallmetar" nowrap>WSW 19 G 26</td><td class="smallmetar" nowrap>10.00</td><td class="smallmetar" nowrap align="left">Mostly Cloudy</td><td class="smallmetar" nowrap align="center">FEW025<br>BKN250</td><td class="smallmetar" nowrap>43</td><td class="smallmetar" nowrap>32</td><td class="smallmetar" nowrap></td><td class="smallmetar" nowrap></td><td class="smallmetar" nowrap></td><td class="smallmetar" nowrap></td><td class="smallmetar" nowrap>64</td><td class="smallmetar" nowrap>30.05</td><td class="smallmetar" nowrap>1017.9</td><td class="smallmetar" nowrap></td><td class="smallmetar" nowrap></td><td class="smallmetar" nowrap></td><td



