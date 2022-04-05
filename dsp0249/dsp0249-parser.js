#!/usr/bin/env node

const fs = require('fs')
const pdf = require('pdf-parse')

let dataBuffer = fs.readFileSync('dsp0249.pdf')

var orig = console.log
console.log = () => {}

pdf(dataBuffer)
.then(function(data) {
    // console.log(data.numpages)
    // console.log(data.text)
    console.log = orig
    text2json(data.text)
})
.catch(function(error) {
    console.log = orig
    console.log(error)
})

function text2json(text) {

    var lines = text.split('\n')

    lines = filterOutNnnecessary(lines)

    // lines.forEach(line => {
    //     console.log(line)
    // })

    lines = combineMultipleLines(lines)

    // lines.forEach(line => {
    //     console.log(line)
    // })

    convert(lines)
}

function filterOutNnnecessary(lines) {
    for (let i = 0; i < lines.length; i++) {
        if (lines[i].includes('Enumeration Description')) {
            lines = lines.slice(i+1)
            break
        }
    }

    for (let i = 0; i < lines.length; i++) {
        if (lines[i].includes('7 PLDM Entity ID codes 286')) {
            lines = lines.slice(0, i)
            break
        }
    }

    lines = lines.filter(line => {
        if (line.includes('DSP0249 Platform Level Data Model (PLDM) State Set Specification')) {
            return false
        }
        if (line.includes('Published Version 1.1.0')) {
            return false
        }
        if (line.includes('Version 1.1.0 Published')) {
            return false
        }
        if (line.includes('Platform Level Data Model (PLDM) State Set Specification DSP0249')) {
            return false
        }
        if (line.trim() === '') {
            return false
        }
        if (/^\s*Set\s*$/.test(line)) {
            return false
        }
        if (/^\s*ID\s*$/.test(line)) {
            return false
        }
        if (/^\s*PLDM State Set\s*$/.test(line)) {
            return false
        }
        if (/^\s*Enumeration List\s*$/.test(line)) {
            return false
        }
        if (/^\s*PLDM State Set Description\s*$/.test(line)) {
            return false
        }
        if (/^\s*Enumeration Description\s*$/.test(line)) {
            return false
        }
        if (/^Table \d+ – .*$/.test(line)) {
            return false
        }

        return true
    })

    return lines
}

function combineMultipleLines(lines) {
    for (let i = 0; i < lines.length; i++) {

        let str = lines[i]

        if (/^\d+ [A-Z].*|\d+\s*$/.test(str)) {
            let next = lines[i + 1]
            while (typeof(next) === 'string' &&
                !/^\d+.*$/.test(next) &&
                !/^ +\d+ – .*$/.test(next)) {
                    lines[i] += next
                    lines.splice(i+1, 1)
                    next = lines[i + 1]
            }
        }

        if (/^\s?\d+ – [A-Z].*$/.test(str)) {
            let next = lines[i + 1]
            while (typeof(next) === 'string' &&
                !/^\d+.*$/.test(next) &&
                !/^\s?\d+ – .*$/.test(next)) {
                    lines[i] += next
                    lines.splice(i+1, 1)
                    next = lines[i + 1]
            }
        }
    }
    return lines
}

function convert(lines) {

    var obj = {}

    for (let i = 0; i < lines.length; i++) {

        var str = lines[i]

        if (/^\d+ [A-Z].*$/.test(str)) {

            let result = str.match(/(\d+)\s+(([A-Z].*?\s+)+)(.*)/)
            let id = result[1]
            // let re = new RegExp(`${result[3]}$`)
            let name = (result[2].replace(result[3], '')).trim()
            let description = (result[3] + result[4]).trim()
            let state = {}

            let next = lines[i + 1]

            while (typeof(next) === 'string' &&
                /^\s?\d+ – [A-Z].*$/.test(next)) {
                    let result = next.match(/^\s?(\d+) – (([A-Z–].*?\s+)+)(.*)/)
                    let id = result[1].trim()
                    let name = (result[2].replace(result[3], '')).trim()
                    let description = (result[3] + result[4]).trim()

                    state[id] = {
                        name: name,
                        description: description
                    }

                    i++

                    next = lines[i + 1]
            }

            obj[id] = {
                name: name,
                description: description,
                state: state
            }
        }
    }

    let filename = 'state.json'
    fs.writeFileSync(filename, JSON.stringify(obj, null, 2), { mode: 0o644 })
    console.log(`Convert to "${filename}" success!`)
}
