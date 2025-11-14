// server.js
const express = require('express');
const cors = require('cors');
const fs = require('fs');
const { spawn } = require('child_process');

const registerRouter = require('./register'); // register.js
const loginRouter = require('./login');       // login.js
const pool = require('./db');                 // db.js

require('dotenv').config();

const app = express();
const PORT = process.env.PORT || 3000;

// Middleware
app.use(cors());
app.use(express.json());

// --- Auth routes ---
app.use('/auth', registerRouter); // handles /auth/register
app.use('/auth', loginRouter);    // handles /auth/login

// --- Load dictionary for autocomplete ---
const dictionary = fs.readFileSync('dictionary.txt', 'utf-8')
    .split('\n')
    .map(word => word.trim().toLowerCase())
    .filter(Boolean);

// --- Spellcheck route using your C executable ---
app.post('/spellcheck', (req, res) => {
    const inputText = (req.body.text || '').trim();
    if (!inputText) return res.status(400).json({ corrected: "Text is required" });

    // Spawn your C program (Windows example: spellcheck.exe)
    const spellProcess = spawn('./spellcheck.exe');

    let output = '';
    let errorOutput = '';

    spellProcess.stdout.on('data', (data) => {
        output += data.toString();
    });

    spellProcess.stderr.on('data', (data) => {
        errorOutput += data.toString();
    });

    spellProcess.on('close', (code) => {
        if (errorOutput) {
            console.error("C Program Error:", errorOutput);
            return res.status(500).json({ corrected: "Error running spellcheck" });
        }
        res.json({ corrected: output.trim() });
    });

    // Send input to C program
    spellProcess.stdin.write(inputText + '\n');
    spellProcess.stdin.end();
});

// --- Autocomplete route ---
app.post('/autocomplete', (req, res) => {
    const prefix = (req.body.prefix || '').toLowerCase().trim();
    if (!prefix) return res.status(400).json({ suggestions: [] });

    const suggestions = dictionary.filter(word => word.startsWith(prefix)).slice(0, 10);
    res.json({ suggestions });
});

// --- Start server ---
app.listen(PORT, () => {
    console.log(`Server running on http://localhost:${PORT}`);
});
