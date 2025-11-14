// db.js
const mysql = require('mysql2/promise');

// Read from environment variables
const pool = mysql.createPool({
    host: process.env.DB_HOST,       // e.g., Render DB host
    user: process.env.DB_USER,       // DB user
    password: process.env.DB_PASSWORD, // DB password
    database: process.env.DB_NAME,   // DB name
    waitForConnections: true,
    connectionLimit: 10,
    queueLimit: 0
});

module.exports = pool;
