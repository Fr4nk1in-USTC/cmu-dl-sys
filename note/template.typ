#let note(title: "Note title", author: "Name", logo: none, date: none, body) = {
    // Set the document's basic properties.
    set document(author: (author, ), title: title)
    set page(
        numbering: "1", 
        number-align: end,
        // Running header.
        header-ascent: 14pt,
        header: locate(loc => {
            let i = counter(page).at(loc).first()
            if i == 1 { return }
            set text(size: 8pt)
            if calc.odd(i) { align(start, title) } else { align(end, title) }
        }),
    )
    set text(font: "New Computer Modern", lang: "en")
    show math.equation: set text(font: "New Computer Modern Math", weight: 400)

    // Set paragraph spacing.
    show par: set block(above: 1.2em, below: 1.2em)

    set par(leading: 0.75em)

    // Title page.
    // The page can contain a logo if you pass one with `logo: "logo.png"`.
    v(0.6fr)
    if logo != none {
        align(right, image(logo, width: 26%))
    }
    v(9.6fr)

    text(1.1em, date)
    v(1.2em, weak: true)
    text(2em, weight: 700, title)

    // Author information.
    pad(
        top: 0.7em,
        right: 20%,
        align(start, author)
    )

    v(2.4fr)
    pagebreak()

    // Heading
    set heading(numbering: "1.")

    // Table of contents.
    outline(depth: 3, indent: true)
    pagebreak()

    // Main body
    set par(justify: true)

    // Code
    show raw.where(block: false): box.with(
        fill: luma(240),
        inset: (x: 3pt, y: 0pt),
        outset: (y: 3pt),
        radius: 2pt,
    )

    show raw.where(block: true): block.with(
        width: 100%,
        fill: luma(240),
        inset: 10pt,
        radius: 4pt,
    )

    body
}

// Theorem and definition environments.

#let theorem-counter = counter("theorem")
#let theorem(body, numbered: true) = locate(location => {
    let lvl = counter(heading).at(location)
    let i = theorem-counter.at(location).first()
    let top = if lvl.len() > 0 { lvl.first() } else { 0 }
    show: block.with(spacing: 11.5pt)
    stack(
        dir: ttb,
        rect(fill: blue, stroke: blue)[
            #strong(
                text(white)[
                    Theorem
                    #if numbered [ #top.#i.]
                ]
            )
        ],
        rect(width: 100%, stroke: blue)[
            #emph(body)
        ]
    )
})


#let definition-counter = counter("definition")
#let definition(body, numbered: true) = locate(location => {
    let lvl = counter(heading).at(location)
    let i = definition-counter.at(location).first()
    let top = if lvl.len() > 0 { lvl.first() } else { 0 }
    show: block.with(spacing: 11.5pt)
    stack(
        dir: ttb,
        rect(fill: orange, stroke: orange)[
            #strong(
                text(white)[
                    Definition
                    #if numbered [ #top.#i.]
                ]
            )
        ],
        rect(width: 100%, stroke: orange)[
            #emph(body)
        ]
    )
})

// Proof environment
#let proof(body) = block(spacing: 11.5pt, {
  emph[Proof.]
  [ ] + body
  h(1fr)
  box(scale(160%, origin: bottom + right, sym.square.stroked))
})
