ΤΣΙΑΤΟΥΡΑΣ ΕΥΑΓΓΕΛΟΣ
Α.Μ. 1115201200185

Το project πιστεύω πως ειναι πλήρως ολοκληρωμένο. Οι παραδοχές που έχω κάνει είναι οι εξής.
Για να δουλέψει το πρόγραμμα πρέπει ο φάκελος στον οποίο θα αποθηκευτούν όλα τα boards να
υπάρχει ήδη. Ακόμη τα αρχεία που στέλνονται από το boardpost πρός τον server, ο server δεν
αποθηκεύει τα αρχεία με την μορφή χαρακτήρων στη δομή αλλά δημιουργεί προσωρινό αρχείο στον
δίσκο και συγκεκριμένα στο parent directory του board (π.χ. έχουμε ένα board1 στο path
/tmp/sdi1200185/board1, το αρχείο θα αποθηκευτεί στο /tmp/sdi1200185/). Έπειτα αν ο client 
ζητήσει να λάβει τα εισερχόμενα μηνύματα, τα αρχεία θα αποθηκευτούν σε ένα φάκελο ο οποίος θα 
βρίσκεται στο ίδιο path που βρίσκεται το εκτελέσιμο Board (π.χ. εκτελούμε το ./Board στο 
path /home/users/sdi1200185/Desktop, τα εισερχόμενα αρχεία θα αποθηκευτούν στο εξής path
/home/users/sdi1200185/Desktop/<Board_Name> files/<Channel_Number>/). Αυτό γίνεται επειδή
θεώρησα οτί είναι καλό να υπάρχει μια αρχειοθέτηση. Ακόμη ο server χρησιμοποιεί μια δυναμική
δομή δεδομένων η οποία πρώτον αποτελείται απο ενα δυναμικό πίνακα που περιέχει στοιχεία τύπου 
Channel κάθε ενα στοιχείο από αυτά περιέχει το id του channel το όνομα του channel έναν δυναμικό
πίνακα για την αποθήκευση των μηνυμάτων σε μορφή χαρακτήρων και έναν δυναμικό πίνακα για την
αποθήκευση των file descriptors των προσωρινών αρχείων. Όλοι οι δυναμικοί πίνακες όταν γεμίσουν
κάνουν reallocate στο διπλάσιο μέγεθος. Επίσης όλες οι επικοινωνίες μεταξύ των processes γίνονται
μέσα απο τα FIFOs τα οποία για την ακρίβεια είναι 4 στο πλήθος τους και πιο συγκεκριμένα χρειάστηκαν
ένα από τον server στον client, ένα από τον client στον server, ένα από τον server στο boardpost 
και ένα από το boardpost στον server. Τέλος αξίζει να σημειώθει οτι στο Boardpost εχω προσθέσει 
εντολή exit για τον τερματισμό του. (Κάποιες φορές κολλάει η getmessages όταν λαμβάνει αρχεία).

Compile:

Έχω δημιουργήσει 2 project ένα για το Board και ένα για το Boardpost. Σε κάθε φάκελο απλά κάνουμε 
make.

