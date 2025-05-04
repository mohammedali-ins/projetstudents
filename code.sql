CREATE TABLE IF NOT EXISTS notes (
    id INT AUTO_INCREMENT PRIMARY KEY,
    etudiant_id INT,
    matiere VARCHAR(50),
    note FLOAT,
    FOREIGN KEY (etudiant_id) REFERENCES etudiant(id) ON DELETE CASCADE
);
