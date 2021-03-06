# Αναφορά δεύτερου μέρους ανάπτυξης λογισμικού για πληροφοριακά συστήματα

1115201200185 ΒΑΓΓΕΛΗΣ ΤΣΙΑΤΟΥΡΑΣ  
1115201200168 ΝΙΚΟΣ ΣΟΦΡΑΣ

## Παραδοχές & λεπτομέρειες υλοποίησης
### Part 1: Bloom Filter
Για την υλοποίηση του Bloom Filter χρησιμοποιήθηκε η hash function murmur3. Η διαδικασία με την οποία επιτυγχάνεται το φιλτράρισμα των 
ngrams γίνεται με τον εξής τρόπο. Αρχικά δεσμεύται ένας πίνακας για το bloom filer σε ενα αρχικό μέγεθος και στην συνέχεια 
υπολογίζεται σε κάθε query η πιθανότητα να υπαρχεί false positive βάσει του πλήθους του input, δηλαδή χρησιμοποιώντας τον μαθηματικό τύπο που δίνεται 
απο το [wikipedia](https://en.wikipedia.org/wiki/Bloom_filter) ορίζουμε σαν estimated input το 75% του πλήθους των εισερχόμενων λέξεων. 
Αυτή η επιλογή έγινε διότι είναι πολύ ακραίο σενάριο ένα query να βρεθεί αυτούσιο (100% του πλήθους των λέξεων) αποθηκευμένο
μέσα στο trie αν κρίνουμε απο τα input files που έχουν δώθει. Επομένως με αυτόν το τρόπο παίρνουμε ενα "ιδανικό" μέγεθος για το πίνακα του
bloom filter έχοντας μια πιθανότητα (0.001%) σαν φράγμα. Στην συνέχεια, το μέγεθος αυτό που προέκυψε απο τους παραπάνω υπολογισμούς, εάν είναι
μεγαλύτερο από το υπάρχον μέγεθος του πίνακα, γίνεται realloc στο νέο μέγεθος, αλλίως παραμένει ως έχει. Αξίζει να σημειωθέι ότι το
reallocation δεν γίνεται συχνά και ότι χρησιμοποιθηκαν 3 hash functions, οι 2 είναι η murmur3 με διαφορετικά seeds και η 3η προκύπτει μέσω του τύπου
Kirsch-Mitzenmacher-Optimization, όπου ουσιαστικά είναι ένας γρήγορος τρόπος να παράξει κανείς hash values μέσω των υπάρχουσων hash functions.

### Part 2: Linear Hashing
Για την υλοποίση του linear hashing η τακτική που ακολουθήθηκε βασίστηκε στις σημειώσεις που δώθηκαν. Αρχικά το hash table αποτελείται απο 4 buckets τα
οποία έχουν μέγεθος 4. Δεν χρησιμοποιήθηκε load factor.

### Part 3: Top k N-grams
Η λειτουργικότητα για αυτό το μέρος της εργασίας είναι υλοποιημένη στο ngramcounter.c/.h. Αρχικά τα αποτελέσματα από κάθε query είσαγονται 
σε ένα στατικό hash table μεγέθους 1099 μαζί με ένα μετρητή που μετράει πόσες φορές εμφανίζεται το αποτέλεσμα στη ριπή. Όταν πάει να εισαχθεί 
ένα ngram που υπάρχει ήδη τότε απλά αυξάνεται ο μετρητής του. Όταν τελειώσει η ριπή τα ζευγάρια ngram και counter αντιγράφονται σε ένα πίνακα. 
Στη συνέχεια καλείται η [quickselect](https://en.wikipedia.org/wiki/Quickselect), η οποία έχει μέση πολυπλοκότητα O(n) και βρίσκει τη θέση που θα 
υπάρχει το k-οστό στοιχείο και ταξινομεί εν μέρει τον πίνακα. Τέλος, καλείται η quicksort η οποία ταξινομεί τον πίνακα από την πρώτη θέση μέχρι τη 
θέση k με μέση πολυπλοκότητα O(klogk) και τυπώνονται τα αποτελέσματα της ριπής με φθίνουσα σειρά. Μόλις τελειώσει η εκτύπωση διαγράφεται ο πίνακας 
και τα δεδομένα από το στατικό hash table ώστε να είναι έτοιμο για την επόμενη ριπή. Έτσι, με αυτή τη δομή επιτυγχάνεται μέση πολυπλοκότητα O(n) 
όταν το n είναι αρκετά μεγαλύτερο από το k και O(klogk) όταν το k είναι ίδιας τάξης με το n.

### Part 4: Static Trie
Για την υλοποίση του static trie χωρίστηκε το πρόγραμμα σε δύο parsers για τα αρχεία εισόδου, έναν για τα dynamic και έναν για τα static. Επίσης 
προστέθηκε ένα επιπλέον πεδίο στο TrieNode το οποίο είναι ο πίνακας με τα offset των συμπιεσμένων λέξεων και οι συμπιεσμένες λέξεις αποθηκεύονται στο 
πεδίο largeWord. Στη περίπτωση που εκτελείται το πρόγραμμα για dynamic το παραπάνω πεδίο δεν χρησιμοποιέιται και είναι ίσο με NULL. 
Τέλος προσαρμόστηκαν οι συναρτήσεις query_trie και binary_search.

## Compilation

* __make__ παράγει εκτελέσιμο trie
* __make u_test__ παράγει εκτελέσιμο αρχείο για unit testing
* __make clean__ διαγράφει εκτελέσιμα και αντικείμενα αρχεία


## Unit testing

Για το unit testing χρησιμοποιήθηκε το framework [Check](https://libcheck.github.io/check/).
