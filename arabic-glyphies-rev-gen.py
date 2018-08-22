import re;

inp = open("arabic-glyphies-base.h", "r").read();
a = inp.split("=")[1].split(";")[0];
a = a[4:len(a)-2];
a = a.split("\n");
for i in range(len(a)):
    a[i] = re.sub(r"\s*", "", a[i]);
    a[i] = a[i][1:len(a[i])-2];
    a[i] = a[i].split(",");
    for j in range(len(a[i])):
        try:
            a[i][j] = int(a[i][j], 16);
        except:
            a[i] = None;
            break;
a = [i for i in a if i and len(i) == 6 and i[1] != 0];
map = {};
for i in range(len(a)):
    for j in range(1, 5):
        map[a[i][j]] = a[i][0];
keys = list(map.keys());
keys.sort();

j = 0;
ranges = {};
for i in range(len(keys)):
    if (i > 0 and keys[i-1] != keys[i]-1):
        j += 1;
    if (j not in ranges):
        ranges[j] = [];
    ranges[j].append(keys[i]);

a1 = ranges[0] + ranges[1] + ranges[2] + ranges[3] + ranges[4] + ranges[5] + ranges[6];
a2 = ranges[7];
a3 = ranges[8];

ranges2 = { 0: a1, 1: a2, 2: a3 };

o = open("arabic-glyphies.h", "w");
ostr = "";
for i in range(len(ranges2)):
    ostr += "#define ARABIC_GLPHIES_REV_%d_LEN %d\n" % (i, len(ranges2[i]));
    ostr += "#define ARABIC_GLPHIES_REV_%d_MIN 0x%04X\n" % (i, ranges2[i][0]);
    ostr += "#define ARABIC_GLPHIES_REV_%d_MAX 0x%04X\n" % (i, ranges2[i][len(ranges2[i])-1]);
    if i != 0:
        ostr += "int ARABIC_GLPHIES_REV_%d[] = {\n" % i;
    else:
        ostr += "int ARABIC_GLPHIES_REV_%d[][2] = {\n" % i;
    for j in range(len(ranges2[i])):
        if i != 0:
            t = "  /* 0x%04X */ 0x%04X%s\n";
        else:
            t = "  { 0x%04X, 0x%04X }%s\n";
        ostr += (t % (ranges2[i][j], map[ranges2[i][j]], ("," if j < (len(ranges2[i])-1) else "")));
    ostr += ("};\n\n");

o.write(inp.replace("ARABIC_GLPHIES_REV", ostr));
